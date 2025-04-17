#pragma once
#include "config.h"

class DungeonTilemap;
class Player;

// 카메라 시스템
class Camera : public Singleton<Camera>
{
private:
    POINT position;          // 카메라 위치 (타일 단위)
    float zoom;              // 카메라 줌 레벨 (1.0f가 기본)
    
    DungeonTilemap* tilemap; // 타일맵 참조
    Player* player;          // 플레이어 참조
    
    bool isFollowingPlayer;  // 플레이어 추적 여부
    float moveSpeed;         // 카메라 이동 속도
    float zoomSpeed;         // 카메라 줌 속도
    
    float targetZoom;        // 목표 줌 레벨
    POINT targetPosition;    // 목표 위치

public:
    HRESULT Init();
    void Release();
    void Update();
    
    // 타일맵 설정
    void SetTilemap(DungeonTilemap* tilemap);
    
    // 플레이어 설정
    void SetPlayer(Player* player);
    
    // 카메라 위치 설정
    void SetPosition(int x, int y);
    void SetPosition(POINT pos);
    
    // 카메라 위치 가져오기
    POINT GetPosition() { return position; }
    
    // 카메라 이동
    void Move(int deltaX, int deltaY);
    
    // 카메라 줌 설정
    void SetZoom(float zoom);
    
    // 카메라 줌 가져오기
    float GetZoom() { return zoom; }
    
    // 카메라 줌 변경
    void ChangeZoom(float deltaZoom);
    
    // 플레이어 추적 설정
    void SetFollowPlayer(bool follow);
    
    // 플레이어 추적 여부 가져오기
    bool IsFollowingPlayer() { return isFollowingPlayer; }
    
    // 월드 좌표를 화면 좌표로 변환
    POINT WorldToScreen(int worldX, int worldY);
    POINT WorldToScreen(POINT worldPos);
    
    // 화면 좌표를 월드 좌표로 변환
    POINT ScreenToWorld(int screenX, int screenY);
    POINT ScreenToWorld(POINT screenPos);
    
    // 카메라 이동 속도 설정
    void SetMoveSpeed(float speed);
    
    // 카메라 줌 속도 설정
    void SetZoomSpeed(float speed);
    void HandleMouseWheel(int zDelta);

    Camera();
    ~Camera();
};
