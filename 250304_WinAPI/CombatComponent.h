#pragma once
#include "config.h"

class CombatComponent
{
public:
    CombatComponent(int hp, int damage);
    virtual ~CombatComponent();

    // ����: �ڽ��� ��������ŭ target�� �������� ����
    void Attack(CombatComponent* target);

    // ������ ���� ��, hp�� 0 ���ϰ� �Ǹ� ��� ó�� (���� �̺�Ʈ�� Ȯ�� ����)
    void GetAttackDamage(int dmg);

    int GetHp() const { return hp; }
    int GetDamage() const { return damage; }

private:
    int hp;
    int damage;
};