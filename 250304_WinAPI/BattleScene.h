#pragma once
#include "GameObject.h"

class Image;
class Player;
class BattleScene : public GameObject
{
private:
	Image* backGround;
	Image* sampleTile;
	TILE_INFO tileInfo[TILE_X * TILE_Y];

	Player* player;
public:
	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	void Load();

	BattleScene() {};
	virtual ~BattleScene() {};
};

