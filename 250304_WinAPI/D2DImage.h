#pragma once
#include <windows.h>
#include <d2d1.h>
#include <wincodec.h>
#include "config.h"
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "windowscodecs.lib")

class D2DImage {
private:
    static ID2D1Factory* factory;
    static ID2D1HwndRenderTarget* renderTarget;
    static ID2D1SolidColorBrush* brushes[5];
    ID2D1Bitmap* bitmap = nullptr;
    
    // 애니메이션 관련
    int maxFrameX = 0;
    int maxFrameY = 0;
    int frameWidth = 0;
    int frameHeight = 0;
    int currFrameX = 0;
    int currFrameY = 0;
    
public:
#pragma region D2D_LifeCycle
    static HRESULT InitD2D(HWND hWnd);
    static HRESULT BeginDraw();
    static HRESULT EndDraw();
    static void Clear(D2D1::ColorF color);
    static ID2D1HwndRenderTarget* GetRenderTarget();
#pragma endregion

    HRESULT LoadFromFile(const wchar_t* filePath, int maxFrameX = 1, int maxFrameY = 1);

    //x좌표, y좌표, 각도(도),좌우반전,상하반전,투명도(0.0~1.0)
    void Render(float x, float y, float angle = 0.0f,
        bool flipX = false, bool flipY = false, 
        float alpha=1.0f);
    //x좌표, y좌표,현재프레임 ,각도(도),좌우반전,상하반전,투명도(0.0~1.0)
    void RenderFrame(float x, float y, int frameX, int frameY , float angle = 0.0f, 
        bool flipX = false, bool flipY = false, 
        float alpha=1.0f);
    //x좌표, y좌표,가로배율,세로배율,현재프레임 ,각도(도),좌우반전,상하반전,투명도(0.0~1.0)
    void RenderFrameScale(float x, float y,float scaleX,float scaleY,
        int frameX, int frameY, float angle = 0.0f,
        bool flipX = false, bool flipY = false,
        float alpha = 1.0f);
    //좌표의 기준이 중심
    void Middle_Render(float x, float y, float angle = 0.0f, 
        bool flipX = false, bool flipY = false, 
        float alpha = 1.0f);
    //좌표의 기준이 중심
    void Middle_RenderFrame(float x, float y, int frameX, int frameY, float angle = 0.0f, 
        bool flipX = false, bool flipY = false, 
        float alpha = 1.0f);
    //좌표의 기준이 중심
    void Middle_RenderFrameScale(float x, float y, float scaleX, float scaleY,
        int frameX, int frameY, float angle = 0.0f,
        bool flipX = false, bool flipY = false,
        float alpha = 1.0f);
    void RenderPercent(FPOINT pos,float spercent, float epercent, float alpha);
    void InitBrushes();
    void ReleaseBrushes();
    void DrawLine(FPOINT point1, FPOINT point2, int color, float lineThickness);
    void DrawRect(FPOINT leftUp,FPOINT rightDown, int color, float lineThickness);
    void DrawCircle(FPOINT center, float radius, int color, float lineThickness);
    
    void Release();
    inline int GetMaxFrameX() { return maxFrameX; }
    inline int GetMaxFrameY() { return maxFrameY; }
    inline int GetMaxFrame() { return maxFrameX * maxFrameY; }
    inline int GetWidth() { return bitmap->GetSize().width; }
    inline int GetHeight() { return bitmap->GetSize().height; }
    static void ReleaseLast();
};