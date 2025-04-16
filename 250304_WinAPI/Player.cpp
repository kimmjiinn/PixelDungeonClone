#include "Player.h"

#include "CommonFunction.h"
#include "Image.h"

HRESULT Player::Init()
{
    // 플레이어 이미지 로드
    playerImage = ImageManager::GetInstance()->AddImage("iori", TEXT("Image/Iori_walk.bmp"), 612, 104, 9, 1, true, RGB(255, 0, 255));

    // 초기 위치 설정
    x = 19;
    y = 8;
    SetPosition(x, y);

    frame = 0;
    
    return S_OK;
}

void Player::Release()
{

}

void Player::Update()
{
    // frame++;
    // if (frame > 9) frame = 0;
}

// Player.cpp의 Render 함수
void Player::Render(HDC hdc) {
    // if (!playerImage) return;
    playerImage->Render(hdc, screenX, screenY, frame);

}

void Player::SetPosition(int tileX, int tileY)
{
    x = tileX;
    y = tileY;
    
    // 화면 위치 업데이트
    screenX = x * TILE_SIZE + TILE_SIZE / 2;
    screenY = y * TILE_SIZE + TILE_SIZE / 2;
    
    // 이동 애니메이션 초기화
    // isMoving = false;
}

// bool Player::Move(int dx, int dy) {
//     // 이동 가능한지 확인 (맵 경계, 장애물 등)
//     int newX = x + dx;
//     int newY = y + dy;
//     
//     // 맵 경계 검사는 외부에서 이미 수행했다고 가정
//     
//     // 이미 이동 중이면 이동 불가
//     if (isMoving) {
//         return false;
//     }
//     
//     // 이동 애니메이션 시작
//     StartMoveAnimation(newX, newY);
//     
//     return true;
// }
// void Player::StartMoveAnimation(int targetX, int targetY) {
//     // 이동 애니메이션 설정
//     isMoving = true;
//     moveStartX = x;
//     moveStartY = y;
//     moveTargetX = targetX;
//     moveTargetY = targetY;
//     moveProgress = 0.0f;
//     
//     // 위치 업데이트 (이동 즉시 적용)
//     x = targetX;
//     y = targetY;
// }
// void Player::SetHealth(int health)
// {
//     this->health = health;
//     
//     // 체력 범위 제한
//     if (this->health < 0)
//         this->health = 0;
//     if (this->health > maxHealth)
//         this->health = maxHealth;
// }
//
// void Player::TakeDamage(int damage)
// {
//     health -= damage;
//     
//     // 체력 범위 제한
//     if (health < 0)
//         health = 0;
// }
//
// void Player::Heal(int amount)
// {
//     health += amount;
//     
//     // 체력 범위 제한
//     if (health > maxHealth)
//         health = maxHealth;
// }

// void Player::UpdateScreenPosition(Camera* camera)
// {
//     if (!camera)
//         return;
//     
//     // 타일 위치를 월드 위치로 변환
//     float worldX, worldY;
//     
//     if (isMoving)
//     {
//         // 이동 중인 경우 보간된 위치 사용
//         worldX = moveStartX + (moveTargetX - moveStartX) * moveProgress;
//         worldY = moveStartY + (moveTargetY - moveStartY) * moveProgress;
//     }
//     else
//     {
//         // 정지 상태인 경우 타일 중앙 위치 사용
//         worldX = x * TILE_SIZE + TILE_SIZE / 2;
//         worldY = y * TILE_SIZE + TILE_SIZE / 2;
//     }
//     
//     // 월드 위치를 화면 위치로 변환
//     POINT screenPos = camera->WorldToScreen(worldX, worldY);
//     screenX = screenPos.x;
//     screenY = screenPos.y;
// }