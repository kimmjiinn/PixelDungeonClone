#include "Camera.h"

Camera::Camera()
    : zoom(1.0f), minZoom(0.5f), maxZoom(2.0f)
{
    position.x = 0;
    position.y = 0;
    screenWidth = 800;  // 기본값
    screenHeight = 600; // 기본값
}

Camera::~Camera()
{
    // 특별히 해제할 자원 없음
}

void Camera::Init(int screenWidth, int screenHeight)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    
    // 기본 위치는 화면 중앙
    position.x = screenWidth / 2;
    position.y = screenHeight / 2;
    
    // 기본 줌 레벨
    zoom = 1.0f;
}

void Camera::Update()
{
    // 현재는 업데이트할 내용 없음
}

void Camera::SetViewport(HDC hdc)
{
    // 카메라 변환 적용 (줌 및 위치)
    // WinAPI에서 카메라 효과를 구현하기 위해 SetViewportOrgEx와 SetMapMode 사용
    
    // 현재 뷰포트 설정 저장
    SaveDC(hdc);
    
    // 뷰포트 원점 설정 (카메라 중심을 화면 중앙으로)
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;
    
    // 카메라 위치를 고려한 오프셋 계산
    int offsetX = centerX - (int)(position.x * zoom);
    int offsetY = centerY - (int)(position.y * zoom);
    
    // 뷰포트 원점 설정
    SetViewportOrgEx(hdc, offsetX, offsetY, NULL);
    
    // 줌 효과를 위한 매핑 모드 설정
    SetMapMode(hdc, MM_ISOTROPIC);
    
    // 논리적 윈도우 크기 설정
    SetWindowExtEx(hdc, screenWidth, screenHeight, NULL);
    
    // 뷰포트 크기 설정 (줌 적용)
    SetViewportExtEx(hdc, (int)(screenWidth * zoom), (int)(screenHeight * zoom), NULL);
}

void Camera::ResetViewport(HDC hdc)
{
    // 카메라 변환 초기화 (저장된 DC 상태로 복원)
    RestoreDC(hdc, -1);
}

void Camera::SetZoom(float zoom)
{
    // 줌 레벨 제한
    if (zoom < minZoom)
        this->zoom = minZoom;
    else if (zoom > maxZoom)
        this->zoom = maxZoom;
    else
        this->zoom = zoom;
}

void Camera::Move(int deltaX, int deltaY)
{
    // 카메라 이동
    position.x += deltaX;
    position.y += deltaY;
}

void Camera::Zoom(float deltaZoom)
{
    // 줌 레벨 조정
    SetZoom(zoom + deltaZoom);
}

POINT Camera::ScreenToWorld(POINT screenPos)
{
    // 화면 좌표를 월드 좌표로 변환
    POINT worldPos;
    
    // 화면 중앙
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;
    
    // 화면 중앙 기준 오프셋
    int offsetX = screenPos.x - centerX;
    int offsetY = screenPos.y - centerY;
    
    // 줌 레벨 고려하여 월드 좌표 계산
    worldPos.x = position.x + offsetX / zoom;
    worldPos.y = position.y + offsetY / zoom;
    
    return worldPos;
}

POINT Camera::WorldToScreen(POINT worldPos)
{
    // 월드 좌표를 화면 좌표로 변환
    POINT screenPos;
    
    // 화면 중앙
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;
    
    // 카메라 위치 기준 오프셋
    int offsetX = (int)((worldPos.x - position.x) * zoom);
    int offsetY = (int)((worldPos.y - position.y) * zoom);
    
    // 화면 좌표 계산
    screenPos.x = centerX + offsetX;
    screenPos.y = centerY + offsetY;
    
    return screenPos;
}