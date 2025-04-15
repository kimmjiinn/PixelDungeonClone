#include "AstarScene.h"
#include "CommonFunction.h"
#include "algorithm"

HRESULT AstarTile::Init()
{
	return E_NOTIMPL;
}

HRESULT AstarTile::Init(int idX, int idY)
{
	this->idX = idX;
	this->idY = idY;

	center.x = idX * ASTAR_TILE_SIZE + (ASTAR_TILE_SIZE / 2);
	center.y = idY * ASTAR_TILE_SIZE + (ASTAR_TILE_SIZE / 2);

	rc.left = idX * ASTAR_TILE_SIZE;
	rc.right = rc.left + ASTAR_TILE_SIZE;
	rc.top = idY * ASTAR_TILE_SIZE;
	rc.bottom = rc.top + ASTAR_TILE_SIZE;

	costFromStart = 0.0f;
	costToGoal = 0.0f;
	totalCost = 0.0f;

	type = AstarTileType::None;

	parentTile = nullptr;

	color = RGB(100, 100, 100);
	hBrush = CreateSolidBrush(color);

	return S_OK;
}

void AstarTile::Release()
{
	DeleteObject(hBrush);
}

void AstarTile::Update()
{
}

void AstarTile::Render(HDC hdc)
{
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	FillRect(hdc, &rc, hBrush);
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

	SelectObject(hdc, hOldBrush);
}

void AstarTile::SetColor(COLORREF color)
{
	this->color = color;
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(color);
}

void AstarTile::SetCost(const AstarTile* startTile, const AstarTile* destTile)
{
	if (parentTile)
	{
		costFromStart = parentTile->costFromStart + GetDistance({ (float)idX, (float)idY }, { (float)parentTile->idX, (float)parentTile->idY });
	}
	else
	{
		costFromStart = GetDistance({ (float)idX, (float)idY }, { (float)startTile->idX, (float)startTile->idY });
	}

	costToGoal = GetDistance({ (float)idX, (float)idY }, { (float)destTile->idX, (float)destTile->idY });

	totalCost = costFromStart + costToGoal;
}

HRESULT AstarScene::Init()
{
	for (int i = 0; i < ASTAR_TILE_COUNT; i++)
	{
		for (int j = 0; j < ASTAR_TILE_COUNT; j++)
		{
			map[i][j].Init(j, i);
		}
	}

	startTile = &(map[3][3]);
	startTile->SetColor(RGB(255, 0, 0));
	startTile->SetType(AstarTileType::Start);

	currTile = startTile;

	destTile = &(map[10][19]);
	destTile->SetColor(RGB(0, 0, 255));
	destTile->SetType(AstarTileType::End);

	//
	trc = GetRect(0, 0, 20, 20);
	isTarget = false;
	prevTargetPos = { 0, 0 };
	currTargetPos = { 0, 0 };
	speed = 1;
	changeColor = 50;

	return S_OK;
}

void AstarScene::Release()
{
}

void AstarScene::Update()
{
	if (PtInRect(&trc, g_ptMouse))		// 타겟 사각형
	{
		if (KeyManager::GetInstance()->IsStayKeyDown(VK_LBUTTON))
		{
			int tempX = currTargetPos.x;
			int tempY = currTargetPos.y;

			float posX = g_ptMouse.x;
			float posY = g_ptMouse.y;

			UpdateRect(trc, { posX, posY });

			currTargetPos.x = posX / ASTAR_TILE_SIZE;
			currTargetPos.y = posY / ASTAR_TILE_SIZE;
			map[tempY][tempX].SetType(AstarTileType::None);
			map[tempY][tempX].SetColor(RGB(100, 100, 100));
		}

		map[currTargetPos.y][currTargetPos.x].SetType(AstarTileType::Target);
		map[currTargetPos.y][currTargetPos.x].SetColor(RGB(200, 200, 0));
	}

	if (KeyManager::GetInstance()->IsStayKeyDown(VK_RBUTTON))
	{
		// g_ptMouse로 인덱스를 계산
		int x, y;
		x = g_ptMouse.x / ASTAR_TILE_SIZE;
		y = g_ptMouse.y / ASTAR_TILE_SIZE;

		if (0 <= x && x < ASTAR_TILE_COUNT &&
			0 <= y && y < ASTAR_TILE_COUNT)
		{
			if (map[y][x].GetType() != AstarTileType::Start &&
				map[y][x].GetType() != AstarTileType::End)
			{
				map[y][x].SetColor(RGB(100, 150, 100));
				map[y][x].SetType(AstarTileType::Wall);
			}
		}
	}

	// TODO 
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		FindPath();
	}

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RETURN))
	{
		if (pathList.top()->parentTile == startTile)
		{
			for (int i = 0; i < speed - 1; i++)
			{
				pathList.pop();
			}
			pathList.top()->SetColor(RGB(255, 255, 255));
			for (int i = 0; i < speed; i++)
			{
				if (pathList.size() == 1) continue;
				pathList.pop();
			}
		}
		else if (pathList.top() == destTile)
		{
			pathList.top()->SetColor(RGB(255 - changeColor, 255 - changeColor, 255 - changeColor));
			changeColor -= 10;
		}
		else
		{
			pathList.top()->SetColor(RGB(255, 255, 255));

			for (int i = 0; i < speed; i++)
			{
				if (pathList.size() == 1) continue;
				pathList.pop();
			}
		}
	}
}

void AstarScene::Render(HDC hdc)
{
	for (int i = 0; i < ASTAR_TILE_COUNT; i++)
	{
		for (int j = 0; j < ASTAR_TILE_COUNT; j++)
		{
			map[i][j].Render(hdc);
		}
	}
	Rectangle(hdc, trc.left, trc.top, trc.right, trc.bottom);
}

void AstarScene::FindPath()
{
	if (currTile)
	{
		AddOpenList(currTile);
		currTile = GetLowestCostTile();
		closeList.push_back(currTile);

		if (currTile == destTile)
		{
			PrintPath();
			return;

		}

		if (!isTarget)
		{
			LookAround(currTile);
		}
		if (isTarget)
		{
			if ((prevTargetPos.x != currTargetPos.x) || (prevTargetPos.y != currTargetPos.y))
			{
				SetTarget(currTargetPos.x, currTargetPos.y);
				prevTargetPos = currTargetPos;
			}
		}

			FindPath();


		if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RETURN))
		{
			SceneManager::GetInstance()->ChangeScene("전투씬_1", "로딩_1");
		}
	}
}

void AstarScene::AddOpenList(AstarTile* currTile)
{
	static const int dx[8] = { -1, 1, -1, 1, -1, 1, 0, 0 };
	static const int dy[8] = { -1, 1, 1, -1, 0, 0, -1, 1 };

	for (int i = 0; i < 8; i++)
	{
		int nx = currTile->idX + dx[i];
		int ny = currTile->idY + dy[i];
		float moveCost = (abs(dx[i]) + abs(dy[i]) == 2) ? 1.4142f : 1.0f;

		if (nx < 0 || nx >= ASTAR_TILE_COUNT ||
			ny < 0 || ny >= ASTAR_TILE_COUNT)
			continue;

		AstarTile* neighbor = &map[ny][nx];

		if (!isValidNeighbor(neighbor)) continue;

		// 대각선 이동 시 인접 벽 검사
		if (abs(dx[i]) + abs(dy[i]) == 2) {
			bool wallX = map[currTile->idY][nx].GetType() == AstarTileType::Wall;
			bool wallY = map[ny][currTile->idX].GetType() == AstarTileType::Wall;
			if (wallX || wallY) continue;
		}
		// 이동 비용 확인 및 갱신 여부 확인
		UpdateNeighborCosts(neighbor, currTile, moveCost);
	}

}

void AstarScene::UpdateNeighborCosts(AstarTile* neighbor, AstarTile* current, float moveCost)
{
	float newG = current->costFromStart + moveCost;
	bool inOpen = find(openList.begin(), openList.end(), neighbor) != openList.end();

	if (inOpen == false || newG < neighbor->costFromStart)
	{
		neighbor->costFromStart = newG;
		neighbor->costToGoal = Heuristic(neighbor, destTile);
		neighbor->totalCost = neighbor->costFromStart + neighbor->costToGoal;
		neighbor->parentTile = current;

		if (!inOpen)
		{
			neighbor->SetColor(RGB(255, 0, 255));
			openList.push_back(neighbor);
		}
	}
}

float AstarScene::Heuristic(AstarTile* a, AstarTile* b) {
	int dx = abs(a->idX - b->idX);
	int dy = abs(a->idY - b->idY);
	return max(dx, dy) + (1.4142f - 1) * min(dx, dy);
}
AstarTile* AstarScene::GetLowestCostTile()
{
	auto iter = min_element(openList.begin(), openList.end(), [](AstarTile* a, AstarTile* b) {
		return a->totalCost < b->totalCost;
		});

	AstarTile* tile = *iter;
	openList.erase(iter);
	return tile;
}

void AstarScene::PrintPath()
{
	AstarTile* curr = destTile;
	while (curr != nullptr && curr != startTile)
	{
		pathList.push(curr);
		curr->SetColor(RGB(0, 0, 0));
		curr = curr->parentTile;
	}
}

bool AstarScene::isValidNeighbor(AstarTile* neighbor)
{
	bool isNotClose = find(closeList.begin(), closeList.end(), neighbor) == closeList.end();
	return neighbor->GetType() != AstarTileType::Wall && isNotClose;
}

void AstarScene::SetTarget(int x, int y)
{
	if (destTile)
	{
		map[destTile->idY][destTile->idX].SetType(AstarTileType::None);
		map[destTile->idY][destTile->idX].SetColor(RGB(100, 100, 100));
		destTile = nullptr;
	}

	if (0 <= x && x < ASTAR_TILE_COUNT &&
		0 <= y && y < ASTAR_TILE_COUNT)
	{
		if (map[y][x].GetType() != AstarTileType::Start && map[y][x].GetType() != AstarTileType::Wall)	//타겟지점이 도착지점
		{
			destTile = &map[y][x];
			destTile->SetColor(RGB(0, 0, 255));
			destTile->SetType(AstarTileType::End);
		}
	}

	if (startTile)
	{
		map[startTile->idY][startTile->idX].SetType(AstarTileType::None);
		map[startTile->idY][startTile->idX].SetColor(RGB(100, 100, 100));
		startTile = nullptr;
	}

	if (currTile->GetType() != AstarTileType::End && currTile->GetType() != AstarTileType::Wall)
	{
		startTile = currTile;
		startTile->SetColor(RGB(255, 0, 0));
		startTile->SetType(AstarTileType::Start);
	}

	if (!closeList.empty())
	{
		closeList.clear();
	}
	if (!openList.empty())
	{
		openList.clear();
	}
}

void AstarScene::LookAround(AstarTile* currTile)		// 각 방향으로 1타일까지 탐색
{
	static const int dx[8] = { -1, 1, -1, 1, -1, 1, 0, 0 };
	static const int dy[8] = { -1, 1, 1, -1, 0, 0, -1, 1 };

	for (int i = 0; i < 8; i++)
	{
		int nx = currTile->idX + dx[i];
		int ny = currTile->idY + dy[i];

		if (nx < 0 || nx >= ASTAR_TILE_COUNT ||
			ny < 0 || ny >= ASTAR_TILE_COUNT)
			continue;

		if (map[ny][nx].GetType() == AstarTileType::Target)
		{
			isTarget = true;
			SetTarget(nx, ny);
			break;
		}
	}
}
