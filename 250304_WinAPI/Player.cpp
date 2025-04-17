#include "Player.h"
#include "Image.h"
#include "CombatComponent.h"
#include "ImageManager.h"
#include "TimerManager.h"

Player::Player(POINT pos)
{
	this->pos = pos;

	combatComponent = new CombatComponent(this->Info.hp, this->Info.damage);
}

Player::~Player()
{
}

HRESULT Player::Init()
{
	Super::Init();
	currImage = ImageManager::GetInstance()->AddImage(
		"dalia", L"Image\\Dalia(Child).Textures.bmp", 2736, 48, 57, 1, true, RGB(255, 0, 255));
	return E_NOTIMPL;
}

void Player::Release()
{
	Super::Release();
}

void Player::Update()
{
	Super::Update();
	
	float deltaTime = TimerManager::GetInstance()->GetDeltaTime();

	currTick += deltaTime;

	if (currTick > 0.1f)
	{
		currTick = 0;
		currAnimFrame++;
		if (currAnimFrame >= 57 /*currImage->GetMaxFrameX()*/)
		{
			currAnimFrame = 0;
		}
	}
}

void Player::Render(HDC hdc)
{
	Super::Render(hdc);
	if (currImage)
		currImage->FrameRender(hdc, pos.x, pos.y, currAnimFrame, 0);
}

void Player::Move()
{
}

void Player::TakeTurn()
{
}

void Player::SetState(ActState state)
{
	if (this->state == state)
		return;

	this->state = state;
	UpdateAnimation();
}

void Player::SetDir(Dir dir)
{
	this->dir = dir;
	UpdateAnimation();
}

void Player::UpdateAnimation()
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
