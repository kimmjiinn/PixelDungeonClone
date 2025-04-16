#pragma once
#include "GameObject.h"
#include "config.h"

enum class ActState
{
	IDLE,
	MOVE,
	ATTACK,
	ActStateLength
};
struct StatInfo 
{
	int hp;
	int damage;
	int exp;
	int level;
	int hunger;
	int speed;
};
class Image;
class CombatComponent;
class Actor : public GameObject
{
public:
	using Super = GameObject;
	Actor();
	virtual ~Actor();

	virtual HRESULT Init() override;
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
	virtual void Attack(Actor* target);

	StatInfo GetInfo() { return Info; }
	CombatComponent* GetCombatComponent() { return combatComponent; }

protected:
	StatInfo Info;
	POINT pos;
	POINT size;
	int currAnimFrame;
	float currTick;
	Image* currImage;
	CombatComponent* combatComponent;
};

