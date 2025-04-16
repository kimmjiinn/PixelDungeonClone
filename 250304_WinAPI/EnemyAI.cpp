#include "EnemyAI.h"




HRESULT AI::Init()
{
    if (enemy)
    {
        enemyInFov = false;
        justAlerted = false;

        aiStatus = "None";
        vecAiSelector.resize(2);
        vecAiSelector[0] = new Hunting;
        vecAiSelector[1] = new Wandering;

        aiCurrState = vecAiSelector[1];
        return S_OK;
    }

    return E_FAIL;
}

void AI::Release()
{
}

void AI::Update()
{
    // 같은 스테이트면 유지시키다가 바뀌어야 하는 순간에 바꿈
}

void AI::Render(HDC hdc)
{
}

bool AI::canAttack(Enemy* enemy)
{
    // 이동할 수 있는 만큼 갔을 때 플레이어 타일까지 간다면 공격할 수 있다. 즉, 이동타일 범위 안에 플레이어가 있으면 공격 가능

    return false;   // 공격할 수 없다.
}

bool AI::canSee()   // Fov
{

    return false;   // 볼 수 없다.
}

bool AI::act(bool enemyInFov, bool justAlerted)     // 이 fov가 화면 fov인가..
{


    return false;
}

AI::~AI()
{
}

//
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

bool Hunting::act(string& status)
{

    return false;
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

bool Wandering::act(string& status)
{
   // 아무지점이나 목표로 잡아서 움직임


    return true;
}


