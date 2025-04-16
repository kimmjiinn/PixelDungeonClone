#include "Camera.h"
#include "DungeonTilemap.h"
#include "Player.h"
#include "KeyManager.h"

Camera::Camera()
    : zoom(1.0f)
    , tilemap(nullptr)
    , player(nullptr)
    , isFollowingPlayer(true)
    , moveSpeed(5.0f)
    , zoomSpeed(0.1f)
    , targetZoom(1.0f)
{
    position = { 0, 0 };
    targetPosition = { 0, 0 };
}

Camera::~Camera()
{
    Release();
}

HRESULT Camera::Init()
{
    // 초기 설정
    zoom = 1.0f;
    targetZoom = 1.0f;
    isFollowingPlayer = true;
    moveSpeed = 5.0f;
    zoomSpeed = 0.2f;
    
    // 초기 위치는 맵 중앙
    if (tilemap)
    {
        position.x = tilemap->GetMapWidth() / 2;
        position.y = tilemap->GetMapHeight() / 2;
    }
    else
    {
        position = { 0, 0 };
    }
    
    targetPosition = position;
    
    return S_OK;
}

void Camera::Release()
{
    // 참조만 하므로 해제할 것 없음
    tilemap = nullptr;
    player = nullptr;
}

void Camera::Update()
{
    // 플레이어 추적
    if (isFollowingPlayer && player)
    {
        targetPosition = player->GetPosition();
    }
    
    // 키보드 입력으로 카메라 이동
    if (KeyManager::GetInstance()->IsStayKeyDown(VK_LEFT))
    {
        targetPosition.x -= 1;
    }
    else if (KeyManager::GetInstance()->IsStayKeyDown(VK_RIGHT))
    {
        targetPosition.x += 1;
    }
    
    if (KeyManager::GetInstance()->IsStayKeyDown(VK_UP))
    {
        targetPosition.y -= 1;
    }
    else if (KeyManager::GetInstance()->IsStayKeyDown(VK_DOWN))
    {
        targetPosition.y += 1;
    }
    
    // 키보드 입력으로 카메라 줌
    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_ADD) || 
        KeyManager::GetInstance()->IsOnceKeyDown(VK_OEM_PLUS))
    {
        targetZoom += zoomSpeed;
    }
    else if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SUBTRACT) || 
             KeyManager::GetInstance()->IsOnceKeyDown(VK_OEM_MINUS))
    {
        targetZoom -= zoomSpeed;
    }
    
    // 마우스 휠 입력 처리 (WM_MOUSEWHEEL 메시지는 MainProc에서 처리)
    // 이 부분은 MainGame.cpp의 MainProc 함수에서 구현됩니다
    
    // 줌 레벨 제한 (0.5 ~ 2.0)
    targetZoom = max(0.5f, min(2.0f, targetZoom));
    
    // 부드러운 카메라 이동
    float deltaTime = TimerManager::GetInstance()->GetDeltaTime();
    
    // 위치 보간
    position.x += (targetPosition.x - position.x) * moveSpeed * deltaTime;
    position.y += (targetPosition.y - position.y) * moveSpeed * deltaTime;
    
    // 줌 보간
    zoom += (targetZoom - zoom) * zoomSpeed * 10.0f * deltaTime;
    
    // 맵 범위 제한
    if (tilemap)
    {
        int mapWidth = tilemap->GetMapWidth();
        int mapHeight = tilemap->GetMapHeight();
        
        position.x = max(0, min(mapWidth - 1, position.x));
        position.y = max(0, min(mapHeight - 1, position.y));
    }
    
    // 타일맵에 카메라 정보 업데이트
    if (tilemap)
    {
        tilemap->SetCameraPos(position.x, position.y);
        tilemap->SetCameraZoom(zoom);
    }
}

void Camera::SetTilemap(DungeonTilemap* tilemap)
{
    this->tilemap = tilemap;
    
    // 타일맵이 설정되면 카메라 위치도 업데이트
    if (tilemap)
    {
        position.x = tilemap->GetMapWidth() / 2;
        position.y = tilemap->GetMapHeight() / 2;
        targetPosition = position;
    }
}

void Camera::SetPlayer(Player* player)
{
    this->player = player;
    
    // 플레이어가 설정되면 카메라 위치도 업데이트
    if (player && isFollowingPlayer)
    {
        position = player->GetPosition();
        targetPosition = position;
    }
}

void Camera::SetPosition(int x, int y)
{
    position.x = x;
    position.y = y;
    targetPosition = position;
    
    // 맵 범위 제한
    if (tilemap)
    {
        int mapWidth = tilemap->GetMapWidth();
        int mapHeight = tilemap->GetMapHeight();
        
        position.x = max(0, min(mapWidth - 1, position.x));
        position.y = max(0, min(mapHeight - 1, position.y));
        targetPosition = position;
    }
    
    // 타일맵에 카메라 정보 업데이트
    if (tilemap)
    {
        tilemap->SetCameraPos(position.x, position.y);
    }
}

void Camera::SetPosition(POINT pos)
{
    SetPosition(pos.x, pos.y);
}

void Camera::Move(int deltaX, int deltaY)
{
    targetPosition.x += deltaX;
    targetPosition.y += deltaY;
    
    // 플레이어 추적 중이면 추적 해제
    if (deltaX != 0 || deltaY != 0)
    {
        isFollowingPlayer = false;
    }
}

void Camera::SetZoom(float zoom)
{
    this->zoom = max(0.5f, min(2.0f, zoom));
    targetZoom = this->zoom;
    
    // 타일맵에 카메라 정보 업데이트
    if (tilemap)
    {
        tilemap->SetCameraZoom(this->zoom);
    }
}

void Camera::ChangeZoom(float deltaZoom)
{
    targetZoom += deltaZoom;
    targetZoom = max(0.5f, min(2.0f, targetZoom));
}

void Camera::SetFollowPlayer(bool follow)
{
    isFollowingPlayer = follow;
    
    // 플레이어 추적 설정 시 바로 플레이어 위치로 이동
    if (isFollowingPlayer && player)
    {
        targetPosition = player->GetPosition();
    }
}

POINT Camera::WorldToScreen(int worldX, int worldY)
{
    if (tilemap)
    {
        return tilemap->WorldToScreen(worldX, worldY);
    }
    
    // 타일맵이 없는 경우 기본 변환
    POINT screenPos;
    int tileSize = (int)(TILE_SIZE * zoom);
    
    screenPos.x = (worldX - position.x) * tileSize + WINSIZE_X / 2;
    screenPos.y = (worldY - position.y) * tileSize + WINSIZE_Y / 2;
    
    return screenPos;
}

POINT Camera::WorldToScreen(POINT worldPos)
{
    return WorldToScreen(worldPos.x, worldPos.y);
}

POINT Camera::ScreenToWorld(int screenX, int screenY)
{
    if (tilemap)
    {
        return tilemap->ScreenToWorld(screenX, screenY);
    }
    
    // 타일맵이 없는 경우 기본 변환
    POINT worldPos;
    int tileSize = (int)(TILE_SIZE * zoom);
    
    worldPos.x = (screenX - WINSIZE_X / 2) / tileSize + position.x;
    worldPos.y = (screenY - WINSIZE_Y / 2) / tileSize + position.y;
    
    return worldPos;
}

POINT Camera::ScreenToWorld(POINT screenPos)
{
    return ScreenToWorld(screenPos.x, screenPos.y);
}

void Camera::SetMoveSpeed(float speed)
{
    moveSpeed = max(0.1f, speed);
}

void Camera::SetZoomSpeed(float speed)
{
    zoomSpeed = max(0.01f, speed);
}

// 마우스 휠 입력으로 줌 처리
void Camera::HandleMouseWheel(int zDelta)
{
    // zDelta가 양수면 휠을 위로 굴린 것(줌 인), 음수면 아래로 굴린 것(줌 아웃)
    float zoomChange = (zDelta > 0) ? zoomSpeed : -zoomSpeed;
    ChangeZoom(zoomChange);
}
