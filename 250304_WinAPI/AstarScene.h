#pragma once
#include "GameObject.h"
#include "config.h"
#include <stack>

#define ASTAR_TILE_SIZE		30
#define ASTAR_TILE_COUNT	20

enum class AstarTileType { Start, End, Wall, None, Target };

class AstarTile : public GameObject
{
public:
	int idX, idY;
	POINT center;
	RECT rc;
	AstarTileType type;

	float costFromStart;

	float costToGoal;

	float totalCost;

	AstarTile* parentTile;

	COLORREF color;
	HBRUSH hBrush;
	HBRUSH hOldBrush;


	virtual HRESULT Init();
	HRESULT Init(int idX, int idY);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void SetColor(COLORREF color);
	void SetType(AstarTileType type) { this->type = type; }
	AstarTileType GetType() { return this->type; }

	inline POINT GetId() { return{ idX,idY }; }
	void SetCost(const AstarTile* startTile, const AstarTile* destTile);
	inline void SetParentTile(AstarTile* parentTile) { this->parentTile = parentTile; };
	inline float GetTotalCost() { return totalCost; }

	virtual ~AstarTile() {};

	friend class AstarScene;

};

class AstarScene : public GameObject
{
	AstarTile map[ASTAR_TILE_COUNT][ASTAR_TILE_COUNT];

	AstarTile* startTile;
	AstarTile* destTile;

	AstarTile* currTile;

	vector<AstarTile*> openList;
	vector<AstarTile*> closeList;

	//
	stack<AstarTile*> pathList;
	POINT prevTargetPos;
	POINT currTargetPos;
	RECT trc;
	bool isTarget;
	int changeColor;
	int speed;

public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void FindPath();
	void AddOpenList(AstarTile* currTile);

	void UpdateNeighborCosts(AstarTile* neighbor, AstarTile* current, float moveCost);
	float Heuristic(AstarTile* a, AstarTile* b);
	AstarTile* GetLowestCostTile();
	void PrintPath();
	bool isValidNeighbor(AstarTile* neighbor);

	//
	void SetTarget(int x, int y);
	void LookAround(AstarTile* currTile);
	virtual ~AstarScene() {};
};

