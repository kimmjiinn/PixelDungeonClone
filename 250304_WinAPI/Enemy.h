#pragma once
#include "Actor.h"
#include "config.h"

class Image;
class AI;
class Enemy: public Actor
{
public:
	using Super = Actor;
	Enemy(POINT pos);
	virtual ~Enemy();

	virtual HRESULT Init() override;
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void SetState(ActState state);
	void SetDir(Dir dir);
	void SetPos(POINT pos) { this->pos = pos; }

	void UpdateAnimation();

private:
	Image* imageIdle[4] = {};
	Image* imageMove[4] = {};
	Image* imageAttack[4] = {};

	int speed = 1;
	Dir dir = DIR_DOWN;
	ActState state = ActState::IDLE;
	bool keyPressed = false;

	bool isTarget = false;

	AI* ai;
};
