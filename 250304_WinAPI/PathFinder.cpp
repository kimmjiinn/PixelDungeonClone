#include "PathFinder.h"
#include "TileMap.h"
#include <algorithm>
#include <cmath>

// 방향 벡터 초기화 (8방향)
const int PathFinder::DX[8] = { -1, 0, 1, 0, -1, 1, 1, -1 };
const int PathFinder::DY[8] = { 0, -1, 0, 1, -1, -1, 1, 1 };
const int PathFinder::COST[8] = { 10, 10, 10, 10, 14, 14, 14, 14 };

PathFinder::PathFinder()
    : tileMap(nullptr), startTile(nullptr), destTile(nullptr), currTile(nullptr)
{
}

PathFinder::~PathFinder()
{
    // 특별히 해제할 자원 없음
}

void PathFinder::Init(TileMap* tileMap)
{
    this->tileMap = tileMap;
}

std::vector<POINT> PathFinder::FindPath(AstarTile* startTile, AstarTile* destTile)
{
    std::vector<POINT> path;
    
    // 시작 타일이나 목적지 타일이 없으면 빈 경로 반환
    if (!startTile || !destTile)
        return path;
        
    // 시작 타일과 목적지 타일이 같으면 빈 경로 반환
    if (startTile == destTile)
        return path;
        
    // 멤버 변수 초기화
    this->startTile = startTile;
    this->destTile = destTile;
    this->currTile = startTile;
    
    // 열린 목록과 닫힌 목록 초기화
    Reset();
    
    // A* 알고리즘 시작
    bool pathFound = false;
    
    // 주변 타일 탐색
    AddOpenList(currTile);

    while (!openList.empty())
    {
        // 열린 목록에서 F값이 가장 작은 타일 선택
        std::sort(openList.begin(), openList.end(), [](AstarTile* t1, AstarTile* t2) {
            return t1->totalCost > t2->totalCost;
        });
        
        currTile = openList.back();
        openList.pop_back();
        
        // 목적지에 도달했으면 경로 탐색 종료
        if (currTile == destTile)
        {
            pathFound = true;
            break;
        }
        
        // 현재 타일을 닫힌 목록에 추가
        closeList.push_back(currTile);
        
        //// 주변 타일 탐색
        AddOpenList(currTile);
    }
    
    // 경로가 발견되었으면 경로 구성
    if (pathFound)
    {
        AstarTile* curr = destTile;
        
        // 목적지부터 시작 타일까지 역추적
        while (curr != nullptr && curr != startTile)
        {
            if (!curr->GetParent())
                break;
                
            // 부모 타일이 벽이면 경로 무효화
            if (curr->GetParent()->GetType() == AstarTile::TileType::Wall)
            {
                path.clear();
                break;
            }
            
            // 경로에 타일 중심 좌표 추가
            path.push_back(curr->GetCenter());
            curr = curr->GetParent();
        }
        
        // 경로 방향 뒤집기 (시작 -> 목적지)
        std::reverse(path.begin(), path.end());
    }
    int a = 0;
    return path;
}

void PathFinder::AddOpenList(AstarTile* currTile)
{
    // 8방향 탐색
    for (int i = 0; i < 8; ++i)
    {
        int nextX = currTile->GetIdX() + DX[i];
        int nextY = currTile->GetIdY() + DY[i];
        
        // 타일맵 범위 체크
        if (nextX < 0 || nextX >= TILE_COUNT_X || nextY < 0 || nextY >= TILE_COUNT_Y)
            continue;
            
        AstarTile* nextTile = tileMap->GetTile(nextX, nextY);
        
        // 이동 가능 여부 체크
        if (!CanGo(currTile, nextTile))
            continue;
            
        // 이미 닫힌 목록에 있는 타일은 무시
        auto it1 = std::find(closeList.begin(), closeList.end(), nextTile);
        if (it1 != closeList.end())
            continue;
            
        // G, H, F 값 계산
        float g = currTile->costFromStart + COST[i];
        float h = 10 * sqrt(
            (destTile->GetIdX() - nextTile->GetIdX()) * (destTile->GetIdX() - nextTile->GetIdX()) +
            (destTile->GetIdY() - nextTile->GetIdY()) * (destTile->GetIdY() - nextTile->GetIdY())
        );
        
        // 이미 열린 목록에 있는 타일인지 확인
        auto it2 = std::find(openList.begin(), openList.end(), nextTile);
        if (it2 != openList.end())
        {
            // 더 작은 G값을 가질 수 있으면 갱신
            if ((*it2)->costFromStart > g)
            {
                (*it2)->costFromStart = g;
                (*it2)->totalCost = g + h;
                (*it2)->SetParent(currTile);
            }
        }
        else
        {
            // 열린 목록에 추가
            nextTile->costFromStart = g;
            nextTile->costToGoal = h;
            nextTile->totalCost = g + h;
            nextTile->SetParent(currTile);
            openList.push_back(nextTile);
        }
    }
}

bool PathFinder::CanGo(AstarTile* currTile, AstarTile* nextTile)
{
    // 타일이 없거나 벽이면 이동 불가
    if (!nextTile || nextTile->GetType() == AstarTile::TileType::Wall)
        return false;
        
    // 대각선 이동 시 주변 벽 체크
    int dx = nextTile->GetIdX() - currTile->GetIdX();
    int dy = nextTile->GetIdY() - currTile->GetIdY();
    
    // 대각선 이동이면 주변 타일 체크
    if (dx != 0 && dy != 0)
    {
        AstarTile* tile1 = tileMap->GetTile(currTile->GetIdX() + dx, currTile->GetIdY());
        AstarTile* tile2 = tileMap->GetTile(currTile->GetIdX(), currTile->GetIdY() + dy);
        
        // 주변 타일이 벽이면 대각선 이동 불가
        if ((tile1 && tile1->GetType() == AstarTile::TileType::Wall) &&
            (tile2 && tile2->GetType() == AstarTile::TileType::Wall))
            return false;
    }
    
    return true;
}

void PathFinder::Reset()
{
    // 열린 목록과 닫힌 목록 초기화
    openList.clear();
    closeList.clear();
}
