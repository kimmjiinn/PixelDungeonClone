#pragma once
#include "GameObject.h"
#include "config.h"

class Image;

class Tile: public GameObject
{
public:
    // 타일 타입 열거형
    enum class TileType { None, Start, End, Wall };

    Tile() {};
    virtual ~Tile() {};

    virtual HRESULT Init();
    HRESULT Init(int idX, int idY);
    virtual void Release();
    virtual void Update();
    virtual void Render(HDC hdc);



    // 타일 속성 설정/조회 메서드
    void SetColor(COLORREF color);
    void SetType(TileType type) { this->type = type; }
    TileType GetType() { return this->type; }
    int GetIdX() { return idX; }
    int GetIdY() { return idY; }
    POINT GetCenter() { return center; }

    // 타일 이미지 설정
    void SetTileImage(Image* image) { this->tileImage = image; }
    void SetFrameX(int frameX) { this->frameX = frameX; }
    void SetFrameY(int frameY) { this->frameY = frameY; }

private:
    int idX, idY;           // 타일 인덱스
    POINT center;           // 타일 중심 좌표
    RECT rc;                // 타일 영역
    TileType type;          // 타일 타입

    COLORREF color;         // 타일 색상
    HBRUSH hBrush;          // 타일 브러시
    HBRUSH hOldBrush;       // 이전 브러시

    // 타일 이미지 관련 변수
    Image* tileImage = nullptr;
    int frameX = 0;         // 이미지 프레임 X 인덱스
    int frameY = 0;         // 이미지 프레임 Y 인덱스
};

// 타일맵 크기 상수
#define TILE_SIZE       30
#define TILE_COUNT      20

// Map 클래스: 맵전체를 관리
class Map : public GameObject
{
public:
    Map() {};
    virtual ~Map() {};

    HRESULT Init();
    void Release();
    void Update();
    void Render(HDC hdc);

    // 타일 접근 메서드
    int GetWidth() { return TILE_SIZE* TILE_COUNT; }
    int GetHeight() { return TILE_SIZE * TILE_COUNT; }
    Tile::TileType GetTileType(int x, int y) { return tiles[y][x].GetType(); };


private:
    Tile tiles[TILE_COUNT][TILE_COUNT];  // 타일 배열
};