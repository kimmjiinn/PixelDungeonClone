#pragma once
#include "GameObject.h"
class Image;
class Actor : public GameObject
{
public:
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
public:
	using Super = GameObject;
	Actor();
	virtual ~Actor();

	virtual HRESULT Init() override;
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

private:
	StatInfo Info;
	FPOINT pos;
	POINT size;
	Image* image;
};

