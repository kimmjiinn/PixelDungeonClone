﻿#pragma once
#include "Entity.h"

class Player: public Entity
{
public:
	void Move(int dx, int dy);
};