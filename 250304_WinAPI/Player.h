#pragma once
#include "Entity.h"

class Player: public Entity
{
public:
	Player() { graphicID = 1; };
	virtual ~Player() {};

	void Move(int dx, int dy);
};