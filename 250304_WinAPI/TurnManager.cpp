#include "TurnManager.h"
#include "Game.h"
#include "Entity.h"

void TurnManager::AddActor(Entity* actor)
{
    if(actor->isActive)
    {
        turnQueue.push_back(actor); // 첫번째는 항상 플레이어
    }
}

void TurnManager::ProcessTurns(Game game)
{
    if (turnQueue.empty())
        return;
    if (turnQueue[currentActorIndex]->NeedInput())
    {
        while (currentActorIndex < turnQueue.size())
        {
            Entity* currActor;
            currActor = GetCurrentActor();
            currActor->Act(game);
        }
    }
}

void TurnManager::EndTurn()
{
    currentActorIndex++;
}

Entity* TurnManager::GetCurrentActor()
{
    if(turnQueue.empty())
        return nullptr;

    return turnQueue[currentActorIndex];
}
