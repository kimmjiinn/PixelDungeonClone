#include "TurnManager.h"
#include "Actor.h"

TurnManager::TurnManager()
{
}

TurnManager::~TurnManager()
{

}

void TurnManager::ProcessTurn()
{
	actor->TakeTurn();
}
