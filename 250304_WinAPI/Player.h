#pragma once
#include "Actor.h"
#include "config.h"

class Image;
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

	void SetState(ActState state);
	void SetDir(Dir dir);
	void SetPos(POINT pos) { this->pos = pos; }

	void UpdateAnimation();
private:
	Image* imageIdle[4] = {};
	Image* imageMove[4] = {};
	Image* imageAttack[4] = {};

	int speed = {};
	Dir dir = DIR_DOWN;
	ActState state = ActState::IDLE;
	bool keyPressed = false;
};

