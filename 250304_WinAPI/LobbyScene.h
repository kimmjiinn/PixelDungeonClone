#pragma once
#include "GameObject.h"

class LobbyBackground;
class ExitButton;
class MapButton;
class D2DImage;
class LobbyScene: public GameObject
{
private:
	D2DImage* foreground;
	D2DImage* title;
	D2DImage* mainMapTXT;
	D2DImage* toolMapTXT;
	D2DImage* team2TXT;
	D2DImage* fireball;

	float elapsedTime;
	int curFrame;

	LobbyBackground* background;

	MapButton* mainMapButton;
	MapButton* toolMapButton;
	ExitButton* exitButton;

	RECT dashboardRc;
	RECT mainRc;
	RECT toolRc;

	bool isMainDown;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	LobbyScene();
	virtual ~LobbyScene() {};
};