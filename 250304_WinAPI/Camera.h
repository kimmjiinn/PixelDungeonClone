#pragma once
#include "config.h"

// Camera 클래스: 게임 화면의 카메라 기능 담당
class Camera
{
public:
    Camera();
    ~Camera();
    
    void Init(int screenWidth, int screenHeight);
    void Update();
    
    // 카메라 변환 적용/해제
    void SetViewport(HDC hdc);
    void ResetViewport(HDC hdc);
    
    // 카메라 조작
    void SetPosition(POINT pos) { position = pos; }
    void SetZoom(float zoom);
    void Move(int deltaX, int deltaY);
    void Zoom(float deltaZoom);
    
    // 좌표 변환 (화면 좌표 <-> 월드 좌표)
    POINT ScreenToWorld(POINT screenPos);
    POINT WorldToScreen(POINT worldPos);
    
    // 속성 접근자
    float GetZoom() const { return zoom; }
    POINT GetPosition() const { return position; }
    
private:
    POINT position;     // 카메라 중심 위치
    float zoom;         // 줌 레벨 (1.0f = 기본)
    int screenWidth;    // 화면 너비
    int screenHeight;   // 화면 높이
    
    // 카메라 제한
    float minZoom;      // 최소 줌 레벨
    float maxZoom;      // 최대 줌 레벨
};