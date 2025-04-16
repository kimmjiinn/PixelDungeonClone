#pragma once
#include "config.h"

class CombatComponent
{
public:
    CombatComponent(int hp, int damage);
    virtual ~CombatComponent();

    // 공격: 자신의 데미지만큼 target에 데미지를 적용
    void Attack(CombatComponent* target);

    // 데미지 적용 후, hp가 0 이하가 되면 사망 처리 (추후 이벤트로 확장 가능)
    void GetAttackDamage(int dmg);

    int GetHp() const { return hp; }
    int GetDamage() const { return damage; }

private:
    int hp;
    int damage;
};