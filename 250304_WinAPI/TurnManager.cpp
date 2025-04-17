#include "TurnManager.h"
#include "Actor.h"

TurnManager::TurnManager()
{
}

TurnManager::~TurnManager()
{
}

void TurnManager::AddActor(Actor* actor)
{
    if (actor)
    {
        actors.push_back(actor);
    }
}

void TurnManager::RemoveActor(Actor* actor)
{
    if (actor)
    {
        auto it = std::find(actors.begin(), actors.end(), actor);
        if (it != actors.end())
        {
            actors.erase(it);
        }
    }
}

void TurnManager::ClearActors()
{
    actors.clear();
}

void TurnManager::ProcessTurn()
{
    if (!isActive)
        return;
		
    // 모든 액터의 턴 처리
    for (Actor* actor : actors)
    {
        if (actor)
        {
            actor->TakeTurn();
        }
    }
}