#include "Enemy.h"

#include "AstarScene.h"
#include "Image.h"
#include "ImageManager.h"
#include "TimerManager.h"
#include "EnemyAI.h"
#include "TurnManager.h"
#include "config.h"

Enemy::Enemy(POINT pos)
{
    this->pos = pos;
    Info.hp = 10;
    // Info.maxHp = 10;
    Info.damage = 2;
}

Enemy::~Enemy()
{
    SAFE_DELETE(ai);
}

HRESULT Enemy::Init()
{
    Super::Init();
    currImage = ImageManager::GetInstance()->AddImage("rat_Idle", L"Image/rat_Idle.bmp", 16*4, 16, 4, 1, true, RGB(255, 0, 255));

    // 적 초기 상태 설정
    state = ActState::IDLE;
    dir = DIR_DOWN;
    speed = 1;

    ai = new AI(this);
    ai->Init();
    return S_OK;
}

void Enemy::Release()
{
    Super::Release();
}

void Enemy::Update()
{
    Super::Update();

    float deltaTime = TimerManager::GetInstance()->GetDeltaTime();

    // 애니메이션 업데이트
    currTick += deltaTime;
    if (currTick > 0.1f)
    {
        currTick = 0;
        currAnimFrame++;
        if (currAnimFrame >= currImage->GetMaxFrameX())
        {
            currAnimFrame = 0;
        }
    }

    // 턴 기반 시스템이 활성화된 경우에만 AI 업데이트
    // GetCurrentActor() 의존성 제거
    if (TurnManager::GetInstance()->IsActive())
    {
        ai->Update();
        // TakeTurn()은 TurnManager에서 호출하므로 여기서는 제거
    }
}

void Enemy::Render(HDC hdc)
{
    Super::Render(hdc);
    if (currImage)
        currImage->FrameRender(hdc, pos.x, pos.y, currAnimFrame, 0);
        
    // 적 상태 표시 (HP 등)
    // char statusText[100];
    // sprintf_s(statusText, "HP: %d/%d", Info.hp, Info.maxHp);
    // TextOut(hdc, pos.x - 20, pos.y - 30, statusText, strlen(statusText));
}

void Enemy::SetState(ActState state)
{
    if (this->state == state)
        return;

    this->state = state;
    UpdateAnimation();
}

void Enemy::SetDir(Dir dir)
{
    this->dir = dir;
    UpdateAnimation();
}

void Enemy::UpdateAnimation()
{
    // 상태와 방향에 따라 적절한 애니메이션 설정
    switch (state)
    {
    case ActState::IDLE:
        // 방향에 따른 idle 애니메이션 설정
        switch (dir)
        {
        case DIR_UP:
            currAnimFrame = 0;
            break;
        case DIR_DOWN:
            currAnimFrame = 1;
            break;
        case DIR_LEFT:
            currAnimFrame = 2;
            break;
        case DIR_RIGHT:
            currAnimFrame = 3;
            break;
        }
        break;
        
    case ActState::MOVE:
        // 방향에 따른 move 애니메이션 설정
        switch (dir)
        {
        case DIR_UP:
            currAnimFrame = 0;
            break;
        case DIR_DOWN:
            currAnimFrame = 1;
            break;
        case DIR_LEFT:
            currAnimFrame = 2;
            break;
        case DIR_RIGHT:
            currAnimFrame = 3;
            break;
        }
        break;
        
    case ActState::ATTACK:
        // 방향에 따른 attack 애니메이션 설정
        switch (dir)
        {
        case DIR_UP:
            currAnimFrame = 0;
            break;
        case DIR_DOWN:
            currAnimFrame = 1;
            break;
        case DIR_LEFT:
            currAnimFrame = 2;
            break;
        case DIR_RIGHT:
            currAnimFrame = 3;
            break;
        }
        break;
    }
}

void Enemy::Move()
{
    // 이동 방향에 따라 위치 업데이트
    POINT newPos = pos;
    
    switch (dir)
    {
    case DIR_UP:
        newPos.y -= ASTAR_TILE_SIZE;
        break;
    case DIR_DOWN:
        newPos.y += ASTAR_TILE_SIZE;
        break;
    case DIR_LEFT:
        newPos.x -= ASTAR_TILE_SIZE;
        break;
    case DIR_RIGHT:
        newPos.x += ASTAR_TILE_SIZE;
        break;
    }
    
    // 이동 가능한지 확인 (벽 체크 등은 AstarScene에서 처리)
    pos = newPos;
    
    // 이동 후 상태 업데이트
    SetState(ActState::IDLE);
}

void Enemy::TakeTurn()
{
    // AI 결정에 따라 행동 수행
    // 간단한 구현: 플레이어 방향으로 이동
    
    // 랜덤 방향 결정 (실제로는 AI가 결정)
    int randomDir = rand() % 4;
    switch (randomDir)
    {
    case 0:
        SetDir(DIR_UP);
        break;
    case 1:
        SetDir(DIR_DOWN);
        break;
    case 2:
        SetDir(DIR_LEFT);
        break;
    case 3:
        SetDir(DIR_RIGHT);
        break;
    }
    
    SetState(ActState::MOVE);
    Move();
    
    // 턴 종료 호출 제거
    // TurnManager가 모든 액터의 TakeTurn()을 호출하는 방식으로 변경되었으므로
    // 여기서 EndTurn()을 호출하지 않음
}