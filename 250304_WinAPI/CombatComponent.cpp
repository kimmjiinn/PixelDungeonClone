#include "CombatComponent.h"

CombatComponent::CombatComponent(int hp, int damage) : hp(hp), damage(damage)
{
}

CombatComponent::~CombatComponent()
{
}

void CombatComponent::Attack(CombatComponent* target)
{
    if (target)
    {
        target->GetAttackDamage(damage);
    }
}

void CombatComponent::GetAttackDamage(int dmg)
{
    hp -= dmg;
    if (hp <= 0)
    {
        hp = 0;
    }
}
