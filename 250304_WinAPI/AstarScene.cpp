#include "AstarScene.h"

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

HRESULT AstarScene::Init()
{
	for (int i = 0; i < ASTAR_TILE_COUNT; i++)	// 세로반복 (y)
	{
		for (int j = 0; j < ASTAR_TILE_COUNT; j++)	// 가로반복 (x)
		{
			map[i][j].Init(j, i);
		}
	}

	startTile = &(map[3][3]);
	startTile->SetColor(RGB(255, 0, 0));
	startTile->SetType(AstarTileType::Start);

	currTile = startTile;



	return S_OK;
}

void AstarScene::Release()
{
	//if (startTile)
	//{
	//	startTile->Release();
	//	delete startTile;
	//	startTile = nullptr;
	//}
	//if (destTile)
	//{
	//	destTile->Release();
	//	delete destTile;
	//	destTile = nullptr;
	//}
	//if (currTile)
	//{
	//	currTile->Release();
	//	delete currTile;
	//	currTile = nullptr;
	//}

	//for (auto tile : openList)
	//{
	//	if (tile)
	//	{
	//		tile->Release();
	//		delete tile;
	//	}
	//}
	//openList.clear();
	//for (auto tile : closeList)
	//{
	//	if (tile)
	//	{
	//		tile->Release();
	//		delete tile;
	//	}
	//}
	//closeList.clear();
}

void AstarScene::Update()
{
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_RBUTTON))
	{
		// g_ptMouse로 인덱스를 계산
		int x, y;
		x = g_ptMouse.x / ASTAR_TILE_SIZE;
		y = g_ptMouse.y / ASTAR_TILE_SIZE;

		if (0 <= x && x < ASTAR_TILE_COUNT &&
			0 <= y && y < ASTAR_TILE_COUNT)
		{
			// 시작이나 목적지가 아닐 때
			if (map[y][x].GetType() != AstarTileType::Start &&
				map[y][x].GetType() != AstarTileType::End)
			{
				map[y][x].SetColor(RGB(100, 150, 100));
				map[y][x].SetType(AstarTileType::Wall);
			}
		}
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
	{
		FindPath();
	}
	if (KeyManager::GetInstance()->IsOnceKeyDown('P'))
		PrintPath();
	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
	{
		int idx = g_ptMouse.x / ASTAR_TILE_SIZE;
		int idy = g_ptMouse.y / ASTAR_TILE_SIZE;
		destTile = &(map[idy][idx]);
		destTile->SetColor(RGB(0, 0, 255));
		destTile->SetType(AstarTileType::End);
		return;
	}

	if (KeyManager::GetInstance()->IsStayKeyDown(VK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("전투씬_1", "로딩_1");
	}
	// TODO 
}

void AstarScene::Render(HDC hdc)
{
	for (int i = 0; i < ASTAR_TILE_COUNT; i++)	// 세로반복 (y)
	{
		for (int j = 0; j < ASTAR_TILE_COUNT; j++)	// 가로반복 (x)
		{
			map[i][j].Render(hdc);
		}
	}
}

FPOINT front[] =
{
	FPOINT{-1, 0}, //UP
	FPOINT{0, -1}, //LEFT
	FPOINT{1, 0}, //DOWN
	FPOINT{0, 1},	//RIGHT
	FPOINT{-1, -1}, //LEFT-UP
	FPOINT{1, -1},	//LEFT-DOWN
	FPOINT{1, 1},	//RIGHT-DOWN
	FPOINT{-1, 1},	//RIGHT-UP
};
int cost[] =
{
	10,
	10,
	10,
	10,
	14,
	14,
	14,
	14
};

void AstarScene::FindPath()
{
	if (!destTile)
		return;
	if (currTile)
	{
		if (currTile == destTile)
			return;
		// 주위에 있는 이동가능한 타일들을 F값 계산 후보에 넣는다.
		AddOpenList(currTile);

		// 후보들 중 F값이 가장 작은 타일을 다음 currTile 선정
		if (openList.empty())
			return;
		sort(openList.begin(), openList.end(), [](AstarTile* t1, AstarTile* t2) {
			return t1->totalCost > t2->totalCost;
			});
		AstarTile* nextTile = openList.back();
		openList.pop_back();

		currTile = nextTile;
		// 반복
		FindPath();
	}
}

void AstarScene::AddOpenList(AstarTile* currTile)
{
	for (int i = 0; i < 8; ++i)
	{
		int nextX = currTile->idX + front[i].x;
		int nextY = currTile->idY + front[i].y;

		if (nextX < 0 || nextX >= ASTAR_TILE_COUNT)
			continue;
		if (nextY < 0 || nextY >= ASTAR_TILE_COUNT)
			continue;

		AstarTile* nextTile = &map[nextY][nextX];

		if (!CanGo(nextTile))
			continue;

		auto it1 = find(closeList.begin(), closeList.end(), nextTile);
		if (it1 != closeList.end())
			continue;

		int g = currTile->costFromStart + cost[i];
		int h = 10 * sqrt((destTile->idX - nextTile->idX) * (destTile->idX - nextTile->idX)
				+ (destTile->idY - nextTile->idY) * (destTile->idY - nextTile->idY));

		auto it2 = find(openList.begin(), openList.end(), nextTile);
		if (it2 != openList.end())
		{
			if ((*it2)->costFromStart > g)
			{
				(*it2)->costFromStart = g;
				(*it2)->totalCost = (*it2)->costFromStart + h;
				(*it2)->SetParent(currTile);
			}
		}
		else			
		{
			nextTile->costFromStart = g;
			nextTile->costToGoal = h;
			nextTile->totalCost = g + h;
			nextTile->SetParent(currTile);
			nextTile->SetColor(RGB(128, 128, 128));
			openList.push_back(nextTile);
		}
		closeList.push_back(currTile);
	}
}

void AstarScene::PrintPath()
{
	AstarTile* curr = destTile;
	path.clear();
	while (curr != nullptr && curr != startTile)
	{
		curr->SetColor(RGB(0, 0, 0));
		path.push_back({ curr->center.x, curr->center.y });
		if (!curr->parentTile)
			return;
		curr = curr->parentTile;
	}
	reverse(path.begin(), path.end());
}

bool AstarScene::CanGo(AstarTile* nextTile)
{
	//타일타입이 벽이면 false
	if (nextTile->GetType() == AstarTileType::Wall)
		return false;
	//주변 벽이면 대각이동 X
	int dx = nextTile->idX - currTile->idX;
	int dy = nextTile->idY - currTile->idY;

	if (map[currTile->idY + dy][currTile->idX].type == AstarTileType::Wall
		&& map[currTile->idY][currTile->idX + dx].type == AstarTileType::Wall)
		return false;
	return true;
}

