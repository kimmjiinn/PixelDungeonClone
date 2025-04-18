#pragma once
#include "config.h"

class Map;
class Player;
class TurnManager;
class Entity;
class Game
{
public:
	void Update();
	void AddActor(Entity* actor);
	void HandleInput();
private:
	Map* map;
	Player* player;
	TurnManager* turnManager;
	vector<Entity*> actors;
};

