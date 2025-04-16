#include "Enemy.h"
#include "Image.h"
#include "ImageManager.h"
#include "TimerManager.h"


Enemy::Enemy(POINT pos)
{
    this->pos = pos;
}

Enemy::~Enemy()
{
}

HRESULT Enemy::Init()
{
	Super::Init();
	currImage = ImageManager::GetInstance()->AddImage(
		"rat_Idle", L"Image/rat_Idle.bmp", 16*4, 16, 4, 1, true, RGB(255, 0, 255));
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
}

void Enemy::Render(HDC hdc)
{
	Super::Render(hdc);
	if (currImage)
		currImage->FrameRender(hdc, pos.x, pos.y, currAnimFrame, 0);
}

void Enemy::SetState(ActState state)
{
	if (state == state)
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
	switch (state)
	{
	case ActState::IDLE:
		break;
	case ActState::MOVE:
		break;
	case ActState::ATTACK:
		break;
	}
}