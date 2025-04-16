#pragma once
#include "GameObject.h"

class Image;
class Player //: public GameObject
{
private:
    int x, y;                  // 타일 기반 위치
    float screenX, screenY;    // 화면상 위치
    Image* playerImage;        // 플레이어 이미지

    int frame;

public:
    Player() = default;
    virtual ~Player() = default;

    virtual HRESULT Init();
    virtual void Release();
    virtual void Update();
    virtual void Render(HDC hdc);

    // 플레이어 위치 설정 및 접근
    void SetPosition(int tileX, int tileY);
    int GetX() const { return x; }
    int GetY() const { return y; }
    
};
