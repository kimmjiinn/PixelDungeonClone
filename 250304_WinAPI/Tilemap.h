#pragma once
#include "AstarTile.h"
#include <vector>

// 타일맵 크기 상수
#define TILE_SIZE       30
#define TILE_COUNT_X    20
#define TILE_COUNT_Y    20

class Image;

// TileMap 클래스: 타일맵 전체를 관리
class TileMap
{
public:
    TileMap();
    ~TileMap();
    
    HRESULT Init();
    void Release();
    void Update();
    void Render(HDC hdc);
    
    // 타일 접근 메서드
    AstarTile* GetTile(int x, int y);
    AstarTile* GetTileFromPos(int posX, int posY);
    
    // 타일 설정 메서드
    void SetTileType(int x, int y, AstarTile::TileType type);
    void LoadTileImages(const wchar_t* imagePath);
    
    // 시작/목적지 타일 설정 및 조회
    void SetStartTile(int x, int y);
    void SetDestTile(int x, int y);
    AstarTile* GetStartTile() { return startTile; }
    AstarTile* GetDestTile() { return destTile; }
    
private:
    AstarTile tiles[TILE_COUNT_Y][TILE_COUNT_X];  // 타일 배열
    Image* tilesetImage;                          // 타일셋 이미지
    
    AstarTile* startTile;                         // 시작 타일
    AstarTile* destTile;                          // 목적지 타일
};
