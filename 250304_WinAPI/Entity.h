#pragma once
#include "config.h"

class MainGame;
class Entity
{
public:
	//int x;
	//int y;
	POINT pos;
	int graphicID;
	bool isActive;

public:
	Entity() {};
	virtual ~Entity() {};

	void SetPosition(int x, int y) { pos.x = x; pos.y = y; };
	//POINT GetPosition() { return { x, y }; };	//원래 형태: Vector2 GetPosition();
	POINT GetPosition() { return pos; }
	virtual void Act(MainGame) = 0;
	virtual bool NeedInput() = 0;
};

