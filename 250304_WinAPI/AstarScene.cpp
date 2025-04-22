#include "AstarScene.h"
#include "CommonFunction.h"
#include "algorithm"
// test
#include "Game.h"

// Fov
#include "FieldOfView.h"

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

	// test
	astarGame = new Game;
	astarGame->Init();

	// fov test
	fov = new FieldOfView;

	return S_OK;
}

void AstarScene::Release()
{
}

void AstarScene::Update()
{
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		SetEntityPos();
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

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_BACK))
	{
		SetVisibleTile();
	}

	astarGame->Update();

	

	// TODO 
	//if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	//{
	//	FindPath();
	//}
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

	astarGame->Render(hdc);
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
		curr->SetColor(RGB(0, 0, 0));
		curr = curr->parentTile;
	}
}

bool AstarScene::isValidNeighbor(AstarTile* neighbor)
{
	bool isNotClose = find(closeList.begin(), closeList.end(), neighbor) == closeList.end();
	return neighbor->GetType() != AstarTileType::Wall && isNotClose;
}

// 맵 함수-이 주소로 세팅. 레벨이 구현되면 레벨에서 이 작업하면 될 것같음.
FPOINT AstarScene::GetRandomFloorTile()
{
	// 랜덤한 타일의 center 좌표를 반환
	// 벽이면 반환하면 안됨
	// 플레이어 주변도 X

	RECT rc;
	rc.left = 0;
	rc.right = ASTAR_TILE_SIZE * 2;
	rc.top = 0;
	rc.bottom = ASTAR_TILE_SIZE * 2;


	int x = (rand() % ASTAR_TILE_COUNT);
	int y = (rand() % ASTAR_TILE_COUNT);

	while ((map[(int)y][(int)x].GetType() == AstarTileType::Wall) ||RectInRect(rc, map[(int)y][(int)x].rc))
	{
		x = (rand() % ASTAR_TILE_COUNT);
		y = (rand() % ASTAR_TILE_COUNT);
	}
			
	FPOINT pos = { map[y][x].center.x, map[y][x].center.y };

	return pos;
}

void AstarScene::SetEntityPos()
{
	// entity 배치
	FPOINT pos = GetRandomFloorTile();
	astarGame->SetEntityOnMap(pos);
}

void AstarScene::ResetVisibleTile()
{
	// 시야 리셋-모든 타일 검사. 맵이 커지면 비효율적일수도 있을 것 같음.
	for (int i = 0; i < ASTAR_TILE_COUNT; i++)
	{
		for (int j = 0; j < ASTAR_TILE_COUNT; j++)
		{
			if (map[i][j].isVisible == true)
			{
				//map[i][j].SetColor(RGB(100, 100, 100));
				if (map[i][j].GetType() == AstarTileType::Wall/* ||
					map[i][j].GetType() == AstarTileType::Start ||
					map[i][j].GetType() == AstarTileType::End*/)
				{
					map[i][j].isVisible = false;
					continue;
				}
				map[i][j].SetColor(RGB(100, 100, 100));
				
			}
		}
	}
}

void AstarScene::SetVisibleTile()
{
	//// 시야 설정-시야에 드는 타일은 색을 다르게 함.
	//// 시야 리셋
	ResetVisibleTile();

	// 임시 좌표-플레이어 좌표
	//map[10][10].SetColor(RGB(200, 200, 0));
	map[10][10].isVisible = true;

	for(int i = 0; i< 8; i++)
	{
		fov->Calculate(reinterpret_cast<AstarTile(&)[20][20]>(map), 10, 10, 0, 
			1.0f, 0.0f, scanDirections[0]);
		int a = 0;
	}
	map[10][10].SetColor(RGB(200, 200, 0));
}
