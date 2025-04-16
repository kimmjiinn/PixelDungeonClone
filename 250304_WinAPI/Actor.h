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
class Image;
class Actor : public GameObject
{
public:
	struct StatInfo 
	{
		int hp;
		int damage;
		int exp;
		int level;
		int hunger;
		int speed;
	};
public:
	using Super = GameObject;
	Actor();
	virtual ~Actor();

	virtual HRESULT Init() override;
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

protected:
	StatInfo Info;
	POINT pos;
	POINT size;
	int currAnimFrame;
	float currTick;
	Image* currImage;
};

