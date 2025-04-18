#include "TurnManager.h"
#include "Game.h"
#include "Entity.h"

void TurnManager::AddActor(Entity* actor)
{
    turnQueue.push_back(actor); // 첫번째는 항상 플레이어
}

void TurnManager::ProcessTurns(Game game)
{
    if (turnQueue.empty())
        return;
}

void TurnManager::EndTurn()
{
}

Entity* TurnManager::GetCurrentActor()
{
    return nullptr;
}
