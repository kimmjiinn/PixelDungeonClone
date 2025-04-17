#pragma once
#include "Actor.h"
#include "config.h"

class Image;
class CombatComponent;
class Player : public Actor
{
public:
	using Super = Actor;

	Player(POINT pos);
	virtual ~Player();

	virtual HRESULT Init() override;
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	void Move() override;
	void TakeTurn();
	void HandleInput();

	void SetState(ActState state);
	void SetDir(Dir dir);
	void SetPos(POINT pos) { this->pos = pos; }

	void UpdateAnimation();

	//
	POINT GetPos() { return pos; }
private:
	Image* imageIdle[4] = {};
	Image* imageMove[4] = {};
	Image* imageAttack[4] = {};

	Dir dir = DIR_DOWN;
	ActState state = ActState::IDLE;
	bool keyPressed = false;
	const int ASTAR_TILE_SIZE = 30;
};
