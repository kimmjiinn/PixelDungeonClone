#pragma once
#include "Singleton.h"
class Actor;
class TurnManager : public Singleton<TurnManager>
{
public:
	TurnManager();
	~TurnManager();

	void ProcessTurn();
	void SetActor(Actor* actor) { this->actor = actor; }
	
private:
	Actor* actor;
};
