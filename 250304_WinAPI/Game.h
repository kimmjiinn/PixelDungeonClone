#pragma once
#include "config.h"

class Entity;
class Player;
class BattleScene;	// map 역할
class TurnManager;

class Game
{
public:
	Game();
	~Game();
	void Update();
	void Render(HDC hdc);
	void AddActor(Entity* actor);
	void HandleInput(WPARAM wParam);
private:
	BattleScene* map;
	Player* player;
	TurnManager* turnManager;
	vector<Entity*> actors;
};

