#include "CombatManager.h"
#include "CombatComponent.h"
#include "Actor.h"

void CombatManager::AddCombatComponent(Actor* actor)
{
    StatInfo info = actor->GetInfo();
    CombatComponent* comp = actor->GetCombatComponent();
    combatList.push_back(comp);
}

void CombatManager::ProccessCombat()
{
    auto attacker = combatList[0];
    auto target = combatList[1];
    if (attacker && target) 
    {
        attacker->Attack(target);
    }
}
