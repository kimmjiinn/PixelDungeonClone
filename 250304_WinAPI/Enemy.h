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
	virtual ~Enemy() override;

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void SetState(ActState state);
	void SetDir(Dir dir);
	void SetPos(POINT pos) { this->pos = pos; }

	void UpdateAnimation();

	void Move() override;
	void TakeTurn() override;
	POINT GetPos() { return pos; }

private:
	Image* imageIdle[4] = {};
	Image* imageMove[4] = {};
	Image* imageAttack[4] = {};

	int speed = 1;
	Dir dir = DIR_DOWN;
	ActState state = ActState::IDLE;
	bool keyPressed = false;

	AI* ai;
	const int ASTAR_TILE_SIZE = 30;
};
