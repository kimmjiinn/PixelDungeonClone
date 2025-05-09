﻿#pragma once
#include "Consumable.h"


class HealPotion : public Consumable
{
private:
	int healAmount = 3;

public:
	HealPotion(FPOINT position);
	virtual ~HealPotion();

	void Use(Entity* user, Level* level) override;
	void Render(HDC hdc) override;
};

