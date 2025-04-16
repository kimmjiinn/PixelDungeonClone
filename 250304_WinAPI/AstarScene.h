#pragma once
#include "GameObject.h"
#include "config.h"
#include <stack>

/*
	���� ��� : Ž���� ������ ����, ���� ��� �߰�
	���� ��� : �̹� Ž���� ������ ����, �ʱ⿡�� ��� ����
	g(n) : ���� ��忡�� ��� n���� �̵��ϴ� ���� ��� 
	h(n) : ��� n���� ��ǥ �������� �������(�޸���ƽ)
	f(n) : ��� n���� ��ǥ ���� �����ϴ� �� ���, f(n) = g(n) + h(n)

	Ž�� ���� :
	 1) ���� ��Ͽ��� f���� ���� ���� ��� n�� �����ؼ� ���� ���� ����
	 2) ���� ��� n�� ��ǥ����� ��� Ž���� �����ϰ�, ��θ� ��ȯ
	 3) �׷��� ������, ���� ��带 ���� ��Ͽ��� �����ϰ� ���� ��Ͽ� �߰�
	 4) ���� ��� n�� ���� ��� �̿���� m�� ���ؼ� ���� �ܰ踦 ����
		4_1) m�� ���� ��Ͽ� ������ ����
		4_2) m�� ���� ��Ͽ� ������ �߰�( g(m), h(m), f(m) / 
			�θ� ���(���� ���)�� n���� ����)
		4_3) m�� �̹� ���� ��Ͽ� �ְ�, ���� ��θ� ���ؼ� �� ���� g����
			���� �� �ִٸ�, g(m), f(m)���� , �θ� ��嵵 ����

	��� ���� :
	��ǥ ��忡�� ���� ������ �θ��带 ��θ� �������ؼ� ��� ����
*/

#define ASTAR_TILE_SIZE		30
#define ASTAR_TILE_COUNT	20

enum class AstarTileType { Start, End, Wall, None, Target };

class AstarTile : public GameObject
{

public:
	virtual ~AstarTile() {};

	virtual HRESULT Init();
	HRESULT Init(int idX, int idY);
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void SetColor(COLORREF color);
	void SetType(AstarTileType type) { this->type = type; }
	void SetParent(AstarTile* tile) { this->parentTile = tile; }
	AstarTileType GetType() { return this->type; }
	AstarTile* GetParent() { return this->parentTile; }
	int GetIdX() { return idX; }
	int GetIdY() { return idY; }

	// enemy
	void SetEnemyParent(AstarTile* tile) { this->enemyParentTile = tile; }

	friend class AstarScene;
public:
	float costFromStart;	// g : ���������� ���� �������� ���
	float costToGoal;		// h : ���� ������ ������������ ������
	float totalCost;		// f : g + h

	//
	float enemyCostFromStart;
	float enemyCostToGoal;
	float enemyTotalCost;

private:
	int idX, idY;
	POINT center;
	RECT rc;
	AstarTileType type;

	AstarTile* parentTile;	// g�� ���ŵ� ������ ���� ��带 ����

	AstarTile* enemyParentTile;

	COLORREF color;
	HBRUSH hBrush;
	HBRUSH hOldBrush;

};

class Player;
class Enemy;
class AstarScene : public GameObject
{
public:
	virtual ~AstarScene() {};
	
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);

	void FindPath();
	void AddOpenList(AstarTile* currTile);
	void PrintPath();
	bool CanGo(AstarTile* nextTile);

	// enemy
	void EnemyFindPath();
	void EnemyPrintPath();
	void AddEnemyOpenList(AstarTile* currTile);
	void SetTarget(Player* player);
	void LookAround();
	void UpdateTargetPos(AstarTile* currTile);
	bool EnemyCanGo(AstarTile* nextTile);

	float currTime = 1.f;
	vector<POINT> path;
	int pathIdx = 0;
	bool moving = false;

	// enemy
	float enemyCurrTime = 1.f;
	vector<POINT> enemyPath;
	int enemyPathIdx = 0;
	bool enemyMoving = false;

public:
	// ������ �迭 ���� ����
	AstarTile map[ASTAR_TILE_COUNT][ASTAR_TILE_COUNT];

	AstarTile* startTile;	// ������
	AstarTile* destTile;	// �Ķ���

	AstarTile* currTile;	// (���) �ĺ�Ÿ���� ������ ���� ����Ÿ��

	vector<AstarTile*> openList;
	vector<AstarTile*> closeList;

	// enemy
	bool isTarget = false;
	Player* target = nullptr;
	AstarTile* enemyStartTile;	
	AstarTile* enemyDestTile;	

	AstarTile* enemyCurrTile;	

	vector<AstarTile*> enemyOpenList;
	vector<AstarTile*> enemyCloseList;

private:
	Player* player;
	Enemy* enemy;
};

