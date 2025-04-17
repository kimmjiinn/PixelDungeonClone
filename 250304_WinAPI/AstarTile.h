#pragma once
#include "GameObject.h"

class Image;

// AstarTile 클래스: 타일맵의 각 타일을 표현
class AstarTile : public GameObject
{
public:
    // 타일 타입 열거형
    enum class TileType { None, Start, End, Wall };
    
    virtual ~AstarTile() {};

    virtual HRESULT Init();
    HRESULT Init(int idX, int idY);
    virtual void Release();
    virtual void Update();
    virtual void Render(HDC hdc);

    // 타일 속성 설정/조회 메서드
    void SetColor(COLORREF color);
    void SetType(TileType type) { this->type = type; }
    void SetParent(AstarTile* tile) { this->parentTile = tile; }
    TileType GetType() { return this->type; }
    AstarTile* GetParent() { return this->parentTile; }
    int GetIdX() { return idX; }
    int GetIdY() { return idY; }
    POINT GetCenter() { return center; }
    
    // 타일 이미지 설정
    void SetTileImage(Image* image) { this->tileImage = image; }
    void SetFrameX(int frameX) { this->frameX = frameX; }
    void SetFrameY(int frameY) { this->frameY = frameY; }
    
    // A* 알고리즘 관련 변수
    float costFromStart;    // g : 시작점부터 현재 노드까지의 비용
    float costToGoal;       // h : 현재 노드부터 목적지까지의 예상비용
    float totalCost;        // f : g + h

private:
    int idX, idY;           // 타일 인덱스
    POINT center;           // 타일 중심 좌표
    RECT rc;                // 타일 영역
    TileType type;          // 타일 타입

    AstarTile* parentTile;  // A* 알고리즘에서 부모 타일
    
    COLORREF color;         // 타일 색상
    HBRUSH hBrush;          // 타일 브러시
    HBRUSH hOldBrush;       // 이전 브러시
    
    // 타일 이미지 관련 변수
    Image* tileImage = nullptr;
    int frameX = 0;         // 이미지 프레임 X 인덱스
    int frameY = 0;         // 이미지 프레임 Y 인덱스
};
