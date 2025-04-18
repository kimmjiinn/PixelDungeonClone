#pragma once
#include "Entity.h"

class Map;
class Image;
class Player: public Entity
{
public:
	Player();
	virtual ~Player() {};

	void Move(int dx, int dy, Map map);


// 렌더에 필요한 것들
public:
	void Update();
	void Render(HDC hdc);

private:
	Image* image;
	int maxFrame;
	int currFrame;
};