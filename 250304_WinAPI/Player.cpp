#include "Player.h"

#include "AstarScene.h"
#include "Image.h"
#include "CombatComponent.h"
#include "ImageManager.h"
#include "TimerManager.h"
#include "KeyManager.h"
#include "TurnManager.h"

Player::Player(POINT pos)
{
	this->pos = pos;
	combatComponent = new CombatComponent(this->Info.hp, this->Info.damage);
}

Player::~Player()
{
	// SAFE_DELETE(combatComponent);
}

HRESULT Player::Init()
{
	Super::Init();
	currImage = ImageManager::GetInstance()->AddImage(
		"dalia", L"Image\\Dalia(Child).Textures.bmp", 2736, 48, 57, 1, true, RGB(255, 0, 255));
	
	// 플레이어 초기 상태 설정
	state = ActState::IDLE;
	dir = DIR_DOWN;
	Info.speed = 5;
	
	return S_OK;
}

void Player::Release()
{
	Super::Release();
}

void Player::Update()
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
	
	// 키 입력 처리 (턴 기반 시스템에서는 TakeTurn에서 처리)
	if (TurnManager::GetInstance()->IsActive())
	{
		HandleInput();
	}
}

void Player::Render(HDC hdc)
{
	Super::Render(hdc);
	if (currImage)
		currImage->FrameRender(hdc, pos.x, pos.y, currAnimFrame, 0);
		
	// 플레이어 상태 표시 (HP 등)
	// char statusText[100];
	// sprintf_s(statusText, "HP: %d/%d", Info.hp, Info.maxHp);
	// TextOut(hdc, pos.x - 20, pos.y - 30, statusText, strlen(statusText));
}

void Player::Move()
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
	
	// 턴 종료
	TurnManager::GetInstance()->EndTurn();
}

void Player::TakeTurn()
{
	// 플레이어 턴에서는 키 입력을 기다림
	// 실제 행동은 HandleInput에서 처리
	SetState(ActState::IDLE);
}

void Player::HandleInput()
{
	KeyManager* keyManager = KeyManager::GetInstance();
	bool actionTaken = false;
	
	// 방향키 입력 처리
	if (keyManager->IsOnceKeyDown(VK_UP))
	{
		// SetDir(DIR_UP);
		SetState(ActState::MOVE);
		actionTaken = true;
	}
	else if (keyManager->IsOnceKeyDown(VK_DOWN))
	{
		// SetDir(DIR_DOWN);
		SetState(ActState::MOVE);
		actionTaken = true;
	}
	else if (keyManager->IsOnceKeyDown(VK_LEFT))
	{
		// SetDir(DIR_LEFT);
		SetState(ActState::MOVE);
		actionTaken = true;
	}
	else if (keyManager->IsOnceKeyDown(VK_RIGHT))
	{
		// SetDir(DIR_RIGHT);
		SetState(ActState::MOVE);
		actionTaken = true;
	}
	
	// 액션 키 입력 처리 (공격 등)
	if (keyManager->IsOnceKeyDown('Z'))
	{
		SetState(ActState::ATTACK);
		actionTaken = true;
	}
	
	// 행동을 취했으면 해당 행동 실행
	if (actionTaken)
	{
		if (state == ActState::MOVE)
		{
			Move();
		}
		else if (state == ActState::ATTACK)
		{
			// 공격 로직 (추후 구현)
			SetState(ActState::IDLE);
			TurnManager::GetInstance()->EndTurn();
		}
	}
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
			currAnimFrame = 4;
			break;
		case DIR_DOWN:
			currAnimFrame = 5;
			break;
		case DIR_LEFT:
			currAnimFrame = 6;
			break;
		case DIR_RIGHT:
			currAnimFrame = 7;
			break;
		}
		break;
		
	case ActState::ATTACK:
		// 방향에 따른 attack 애니메이션 설정
		switch (dir)
		{
		case DIR_UP:
			currAnimFrame = 8;
			break;
		case DIR_DOWN:
			currAnimFrame = 9;
			break;
		case DIR_LEFT:
			currAnimFrame = 10;
			break;
		case DIR_RIGHT:
			currAnimFrame = 11;
			break;
		}
		break;
	}
}
