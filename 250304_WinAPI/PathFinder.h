#pragma once
#include "AstarTile.h"
#include <vector>

class TileMap;

// PathFinder 클래스: A* 알고리즘을 이용한 경로 탐색 담당
class PathFinder
{
public:
    PathFinder();
    ~PathFinder();
    
    void Init(TileMap* tileMap);
    
    // A* 알고리즘 관련 함수
    std::vector<POINT> FindPath(AstarTile* startTile, AstarTile* destTile);
    bool CanGo(AstarTile* currTile, AstarTile* nextTile);
    
private:
    void AddOpenList(AstarTile* currTile);
    void Reset();
    
    TileMap* tileMap;           // 타일맵 참조
    AstarTile* startTile;       // 시작 타일
    AstarTile* destTile;        // 목적지 타일
    AstarTile* currTile;        // 현재 탐색 중인 타일
    
    std::vector<AstarTile*> openList;    // 열린 목록 (탐색할 타일)
    std::vector<AstarTile*> closeList;   // 닫힌 목록 (탐색 완료된 타일)
    
    // 방향 벡터 (8방향)
    static const int DX[8];
    static const int DY[8];
    static const int COST[8];   // 이동 비용 (직선: 10, 대각선: 14)
};
