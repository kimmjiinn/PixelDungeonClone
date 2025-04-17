#include "AstarScene.h"

#include "CommonFunction.h"
#include "Player.h"
#include "Enemy.h"
#include "config.h"

HRESULT AstarTile::Init()
{
	return E_NOTIMPL;
}

HRESULT AstarTile::Init(int idX, int idY)
{
	SetClientRect(g_hWnd, WINSIZE_X, TILEMAPTOOL_Y);
	
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

	//enemy
	enemyParentTile = nullptr;


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

	player = new Player({startTile->center.x, startTile->center.y});
	player->Init();

	// enemy
	enemyStartTile = &(map[18][5]);
	enemyStartTile->SetColor(RGB(125, 0, 0));
	enemyStartTile->SetType(AstarTileType::Start);

	enemyCurrTile = enemyStartTile;

	enemy = new Enemy({ enemyStartTile->center.x, enemyStartTile->center.y });
	enemy->Init();
	//isTarget = true;
	//enemyMoving = true;
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
	SAFE_DELETE(player);

	SAFE_DELETE(enemy);
}

void AstarScene::Update()
{
	float deltaTime = TimerManager::GetInstance()->GetDeltaTime();
	//디버깅용 색 초기화
	for (int i = 0; i < ASTAR_TILE_COUNT; i++)
	{
		for (int j = 0; j < ASTAR_TILE_COUNT; j++)
		{
			map[i][j].costFromStart = 0;
			map[i][j].costToGoal = 0;
			map[i][j].totalCost = 0;
			if (map[i][j].type == AstarTileType::None)
				map[i][j].SetColor(RGB(100, 100, 100));
			else if (map[i][j].type == AstarTileType::End)
				map[i][j].SetColor(RGB(0, 0, 255));
			else if (map[i][j].type == AstarTileType::Start)
				map[i][j].SetColor(RGB(255, 0, 0));
			else if (map[i][j].type == AstarTileType::Wall)
				map[i][j].SetColor(RGB(100, 150, 100));
		}
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
			// 시작이나 목적지가 아닐 때
			if (map[y][x].GetType() != AstarTileType::Start &&
				map[y][x].GetType() != AstarTileType::End)
			{
				map[y][x].SetColor(RGB(100, 150, 100));
				map[y][x].SetType(AstarTileType::Wall);

			}
		}
	}
	startTile->SetColor(RGB(0, 0, 255));

	if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
	{
		int idx = g_ptMouse.x / ASTAR_TILE_SIZE;
		int idy = g_ptMouse.y / ASTAR_TILE_SIZE;

		if (idx < 0 || idx >= ASTAR_TILE_COUNT || idy < 0 || idy >= ASTAR_TILE_COUNT)
			return;

		AstarTile* clickTile = &(map[idy][idx]);
		//클릭한 타일이 벽이면 리턴
		if (clickTile->GetType() == AstarTileType::Wall)
			return;
		// 기존 도착지 NoneType으로 리셋
		if (destTile)
			destTile->SetType(AstarTileType::None);
		// 도착지를 클릭한 타일으로 갱신
		destTile = clickTile;
		destTile->SetType(AstarTileType::End);
		moving = true;

	}

	Reset();

	FindPath();
	PrintPath();

	if (moving)
	{
		if (path.empty())
		{
			return;
		}
		if (pathIdx >= path.size())
		{
			moving = false;
			return;
		}

		currTime += deltaTime;
		if (currTime >= 0.5f)
		{
			currTile = &map[path[pathIdx].y / ASTAR_TILE_SIZE][path[pathIdx].x / ASTAR_TILE_SIZE];
			currTime = 0;
			pathIdx++;
		}
	}

	if (player)
	{
		player->SetPos(currTile->center);
		player->Update();
	}

	if (destTile) destTile->SetColor(RGB(255, 0, 0));


	if (KeyManager::GetInstance()->IsStayKeyDown(VK_RETURN))
	{
		SceneManager::GetInstance()->ChangeScene("전투씬_1");

	}
		// enemy

		if (enemy)
		{
			if (!isTarget)
			{
				LookAround();
			}
			if (target)
			{
				UpdateTargetPos(currTile);
			}

			EnemyReset();
			//enemyOpenList.clear();
			//enemyCloseList.clear();
			//enemyPath.clear();
			//enemyPathIdx = 0;
			//enemyStartTile = enemyCurrTile;

			EnemyFindPath();

			EnemyPrintPath();

			// 위치 업데이트. 플레이어 위치와 같으면 위치 업데이트 안함.
			if (enemyCurrTile != currTile)
			{
				enemy->SetPos(enemyCurrTile->center);
				enemy->Update();
			}
			else
			{
				enemyCurrTile->SetColor(RGB(200, 200, 20));
				enemy->Update();
			}

			if (enemyMoving)
			{
				if (enemyPathIdx >= enemyPath.size())
				{
					return;
				}

				enemyCurrTime += deltaTime;
				if (enemyCurrTime >= 0.5f)
				{
					//currTile->SetColor(RGB(255, 0, 255));

					enemyCurrTile = &map[enemyPath[enemyPathIdx].y / ASTAR_TILE_SIZE][enemyPath[enemyPathIdx].x / ASTAR_TILE_SIZE];
					//currTile->SetColor(RGB(255, 0, 0));

					enemyCurrTime = 0;
					enemyPathIdx++;
				}
			}
		}
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
	if (player)
		player->Render(hdc);

	if (enemy)
		enemy->Render(hdc);
}

FPOINT front[] =
{
	FPOINT{-1, 0},	    //UP 
	FPOINT{0, -1},		//LEFT
	FPOINT{1, 0},		//DOWN
	FPOINT{0, 1},	   	//RIGHT
	FPOINT{-1, -1},		//LEFT-UP
	FPOINT{1, -1},		//LEFT-DOWN
	FPOINT{1, 1},		//RIGHT-DOWN
	FPOINT{-1, 1},		//RIGHT-UP
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
		{
			currTile = startTile;
			currTile->SetType(AstarTileType::None);
			return;
		}
		// 주위에 있는 이동가능한 타일들을 F값 계산 후보에 넣는다.
		AddOpenList(currTile);

		// 후보들 중 F값이 가장 작은 타일을 다음 currTile 선정
		if (openList.empty())
		{
			moving = false;
			currTile = startTile;
			return;
		}
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
			openList.push_back(nextTile);
		}
		closeList.push_back(currTile);
	}
}

void AstarScene::PrintPath()
{
	if (!destTile)
		return;

	AstarTile* curr = destTile;
	path.clear();

	if (currTile == destTile->GetParent())
		path.push_back({ curr->center.x, curr->center.y });	

	while (curr != nullptr && curr != startTile)
	{
		if (!curr->parentTile)
			break;

		if (curr->parentTile->GetType() == AstarTileType::Wall)
		{
			path.clear();
			destTile = nullptr;
			break;
		}

		curr->SetColor(RGB(0, 0, 0));
		curr = curr->parentTile;
		path.push_back({ curr->center.x, curr->center.y });
	}

	if (!path.empty())
	{
		path.pop_back();
	}
	else
	{
		moving = false;
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


void AstarScene::Reset()
{
	startTile->SetType(AstarTileType::None);
	if (destTile) destTile->SetType(AstarTileType::None);
	startTile = currTile;
	currTile->SetType(AstarTileType::Start);

	openList.clear();
	closeList.clear();
	path.clear();

	pathIdx = 0;
}

void AstarScene::EnemyFindPath()
{
	if (!enemyDestTile)
		return;
	if (enemyCurrTile)
	{
		if (enemyCurrTile == enemyDestTile)
		{
			enemyCurrTile = enemyStartTile;
			return;
		}
		// 주위에 있는 이동가능한 타일들을 F값 계산 후보에 넣는다.
		AddOpenList(enemyCurrTile);

		// 후보들 중 F값이 가장 작은 타일을 다음 currTile 선정
		if (enemyOpenList.empty())
			return;
		sort(enemyOpenList.begin(), enemyOpenList.end(), [](AstarTile* t1, AstarTile* t2) {
			return t1->enemyTotalCost > t2->enemyTotalCost;
			});
		AstarTile* nextTile = enemyOpenList.back();
		enemyOpenList.pop_back();

		enemyCurrTile = nextTile;
		// 반복
		EnemyFindPath();

	}
}

void AstarScene::EnemyPrintPath()
{
	if (!enemyDestTile)
		return;
	AstarTile* eCurr = enemyDestTile;
	enemyPath.clear();

	//if (enemyCurrTile == enemyDestTile->GetEnemyParent())
		enemyPath.push_back({ eCurr->center.x, eCurr->center.y });

	while (eCurr != nullptr && eCurr != enemyStartTile)
	{

		if (!eCurr->enemyParentTile)
			return;
		//curr->SetColor(RGB(0, 0, 0));
		eCurr = eCurr->enemyParentTile;
		enemyPath.push_back({ eCurr->center.x, eCurr->center.y });
	}
	enemyPath.pop_back();
	reverse(enemyPath.begin(), enemyPath.end());
}

void AstarScene::AddEnemyOpenList(AstarTile* currTile)
{
	for (int i = 0; i < 8; ++i)
	{
		int nextX = enemyCurrTile->idX + front[i].x;
		int nextY = enemyCurrTile->idY + front[i].y;

		if (nextX < 0 || nextX >= ASTAR_TILE_COUNT)
			continue;
		if (nextY < 0 || nextY >= ASTAR_TILE_COUNT)
			continue;

		AstarTile* nextTile = &map[nextY][nextX];

		if (!EnemyCanGo(nextTile))
			continue;

		auto it1 = find(enemyCloseList.begin(), enemyCloseList.end(), nextTile);
		if (it1 != enemyCloseList.end())
			continue;

		int g = enemyCurrTile->enemyCostFromStart + cost[i];
		int h = 10 * sqrt((enemyDestTile->idX - nextTile->idX) * (enemyDestTile->idX - nextTile->idX)
			+ (enemyDestTile->idY - nextTile->idY) * (enemyDestTile->idY - nextTile->idY));

		auto it2 = find(enemyOpenList.begin(), enemyOpenList.end(), nextTile);
		if (it2 != enemyOpenList.end())
		{
			if ((*it2)->enemyCostFromStart > g)
			{
				(*it2)->enemyCostFromStart = g;
				(*it2)->enemyTotalCost = (*it2)->enemyCostFromStart + h;
				(*it2)->SetEnemyParent(currTile);
			}
		}
		else
		{
			nextTile->enemyCostFromStart = g;
			nextTile->enemyCostToGoal = h;
			nextTile->enemyTotalCost = g + h;
			nextTile->SetEnemyParent(currTile);
			//nextTile->SetColor(RGB(128, 128, 128));
			enemyOpenList.push_back(nextTile);
		}
		enemyCloseList.push_back(currTile);
	}
}

void AstarScene::SetTarget(Player *player)
{
	target = player;
	enemyMoving = true;
	//enemyDestTile = &(map[player->GetPos().y / ASTAR_TILE_SIZE][player->GetPos().x / ASTAR_TILE_SIZE]);
	enemyDestTile = currTile;
}

void AstarScene::LookAround()
{
	static const int dx[8] = { -1, 1, -1, 1, -1, 1, 0, 0 };
	static const int dy[8] = { -1, 1, 1, -1, 0, 0, -1, 1 };

	for (int i = 0; i < 8; i++)
	{
		int nx = enemyCurrTile->idX + dx[i];
		int ny = enemyCurrTile->idY + dy[i];

		if (nx < 0 || nx >= ASTAR_TILE_COUNT ||
			ny < 0 || ny >= ASTAR_TILE_COUNT)
			continue;

		if ((map[ny][nx].center.x == player->GetPos().x) && (map[ny][nx].center.y == player->GetPos().y))
		{
			SetTarget(player);
			break;
		}
	}
}

void AstarScene::UpdateTargetPos(AstarTile* currTile)
{
	enemyDestTile = currTile;/*&(map[player->GetPos().y / ASTAR_TILE_SIZE][player->GetPos().x / ASTAR_TILE_SIZE]);*/
	//destTile->SetColor(RGB(0, 0, 255));
	//enemyDestTile->SetType(AstarTileType::End);
}

bool AstarScene::EnemyCanGo(AstarTile* nextTile)
{
	//타일타입이 벽이면 false
	if (nextTile->GetType() == AstarTileType::Wall)
		return false;
	//주변 벽이면 대각이동 X
	int dx = nextTile->idX - enemyCurrTile->idX;
	int dy = nextTile->idY - enemyCurrTile->idY;

	if (map[enemyCurrTile->idY + dy][enemyCurrTile->idX].type == AstarTileType::Wall
		&& map[enemyCurrTile->idY][enemyCurrTile->idX + dx].type == AstarTileType::Wall)
		return false;
	return true;
}

void AstarScene::EnemyReset()
{
	enemyStartTile->SetType(AstarTileType::None);
	if (enemyDestTile) enemyDestTile->SetType(AstarTileType::None);
	enemyStartTile = enemyCurrTile;
	enemyCurrTile->SetType(AstarTileType::Start);

	enemyOpenList.clear();
	enemyCloseList.clear();
	enemyPath.clear();

	enemyPathIdx = 0;
}
