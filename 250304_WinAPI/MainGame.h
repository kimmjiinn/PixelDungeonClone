#pragma once
#include "GameObject.h"

class Timer;
class Image;
class EnemyManager;
class TilemapTool;
class D2DImage;

// test
class Map;
class Player;
class Monster;

class MainGame : public GameObject
{
private:
	HDC hdc;
	PAINTSTRUCT ps;
	wchar_t szText[128];

	Image* backBuffer;

	TilemapTool* tilemapTool;

	Timer* timer;
	D2DImage* test;
	
	// test
	Map* testMap;
	Player* player;
	Monster* monster;

public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	void Render();

	LRESULT MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

	MainGame();
	~MainGame();
};

