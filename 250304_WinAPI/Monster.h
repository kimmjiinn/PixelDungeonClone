#pragma once
#include "Entity.h"

class Map;
class Monster : public Entity
{
public:
	Monster();
	virtual ~Monster() {};

	void Move(int dx, int dy, Map map);
	virtual void Act(MainGame) override;
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