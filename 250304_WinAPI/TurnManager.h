#pragma once
#include "config.h"

class Entity;
class Game;
class TurnManager
{
public:
	void AddActor(Entity *actor);
	void ProcessTurns(Game game);
	void EndTurn();
	Entity* GetCurrentActor();
private:
	vector<Entity*> turnQueue;
	int currentActorIndex = 0;	// 첫번째는 항상 플레이어
};

