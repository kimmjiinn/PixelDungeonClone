#pragma once
#include "Entity.h"

class BattleScene;
class Image;
class Player: public Entity
{
public:
	Player();
	virtual ~Player() {};

	void Move(int dx, int dy, BattleScene map);
	virtual void Act(Game game) override;
	virtual bool NeedInput() override;

// 렌더에 필요한 것들
public:
	void Update();
	void Render(HDC hdc);

private:
	Image* image;
	int maxFrame;
	int currFrame;
};