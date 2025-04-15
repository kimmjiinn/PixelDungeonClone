#include "Actor.h"

Actor::Actor()
{
}

Actor::~Actor()
{
}

HRESULT Actor::Init()
{
	Super::Init();
	return E_NOTIMPL;
}

void Actor::Release()
{
	Super::Release();
}

void Actor::Update()
{
	Super::Update();
}

void Actor::Render(HDC hdc)
{
	Super::Render(hdc);
}
