#include "EnemyAI.h"
#include "Enemy.h"

enum class Selector
{
    Hunting,
    Wandering,
};


HRESULT AI::Init()
{
    if (enemy)
    {
        enemyInFov = false;
        justAlerted = false;

        aiStatus = "None";
        vecAISelector.resize(2);
        vecAISelector[(int)Selector::Hunting] = new Hunting;
        vecAISelector[(int)Selector::Wandering] = new Wandering;
            
        vecAISelector[(int)Selector::Hunting]->Init();
        vecAISelector[(int)Selector::Wandering]->Init();

        return S_OK;
    }

    return E_FAIL;
}

void AI::Release()
{
    vecAISelector[0]->Release();
    vecAISelector[1]->Release();
    vecAISelector.clear();
}

void AI::Update()
{
    // 같은 스테이트면 유지시키다가 바뀌어야 하는 순간에 바꿈
    
    // enemy 시야 받기.
    


    if (CanSee())   // 시야에 보임
    {
        Act(true, false);
    }
    else
    {
        Act(false, false);
    }
    
}

void AI::Render(HDC hdc)
{
}

bool AI::CanSee()   // Fov
{

    return false;   // 볼 수 없다.
}

bool AI::Act(bool enemyInFov, bool justAlerted)
{
    if (justAlerted)
    {
        vecAISelector[(int)Selector::Hunting]->Act(aiStatus);
        return true;
    }
    else if (enemyInFov)
    {
        vecAISelector[(int)Selector::Hunting]->Act(aiStatus);
        return true;
    }
    else
    {
        vecAISelector[(int)Selector::Wandering]->Act(aiStatus);
        return true;
    }

    return false;
}

AI::~AI()
{
}

// Acting
HRESULT Acting::Init()
{
    return E_NOTIMPL;
}

void Acting::Release()
{
}

void Acting::Update()
{
}

void Acting::Render(HDC hdc)
{
}

// Hunting
HRESULT Hunting::Init()
{
    Super::Init();
    return E_NOTIMPL;
}

void Hunting::Release()
{
    Super::Release();
}

void Hunting::Update()
{
    Super::Update();
}

void Hunting::Render(HDC hdc)
{
    Super::Render(hdc);
}

bool Hunting::Act(string& status)
{
    if(CanAttack())
    {
        Attack();
    }
    else
    {
        Follow();
    }
    return false;
}

bool Hunting::CanAttack()
{
    return false;
}

void Hunting::Follow()
{
}

void Hunting::Attack()
{
}

// Wandering
HRESULT Wandering::Init()
{
    Super::Init();
    return E_NOTIMPL;
}

void Wandering::Release()
{
    Super::Release();
}

void Wandering::Update()
{
    Super::Update();
}

void Wandering::Render(HDC hdc)
{
    Super::Render(hdc);
}

bool Wandering::Act(string& status)
{
   // 아무지점이나 목표로 잡아서 움직임
    if (RandomDestination()) // true일 경우 제자리인 거임
    {
        Sleep();
    }
    else
    {
        Move();
    }

    return true;
}

bool Wandering::RandomDestination()
{
    // 현재 위치 받아서 8방향 중에 갈 곳 찾기.

    return false;
}

void Wandering::Move()
{

}

void Wandering::Sleep()
{
}


