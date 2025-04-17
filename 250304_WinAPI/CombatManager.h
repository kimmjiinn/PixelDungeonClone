#pragma once
#pragma once
#include "config.h"

class CombatComponent;
class Actor;
class CombatManager 
{
public:
    void AddCombatComponent(Actor* actor);

    void ProccessCombat();

private:
    vector<CombatComponent*> combatList;
};

