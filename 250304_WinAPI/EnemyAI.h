#pragma once
#include "config.h"

// 위치, 애니메이션 정도 바뀔듯
class Enemy;
class Acting;

struct Node
{
	POINT pos;      // center값으로 보정하기.
	float costFromStart;
	float costToGoal;
	float totalCost;

	Node* parentTile;

	// pos 범위: (ASTAR_TILE_SIZE / 2) ~ ASTAR_TILE_COUNT * ASTAR_TILE_SIZE + (ASTAR_TILE_SIZE / 2)
};

class AI
{
private:
	bool enemyInFov;
	bool justAlerted;
	string aiStatus;
	vector<Acting*> vecAiSelector;
	Acting* aiCurrState;
	Enemy* enemy;

	Node* startTile;
	Node* destTile;
	Node* currTile;

	vector<POINT> openList;


public:
	AI(Enemy* enemy) : enemy(enemy) {};
	// 여기서 이동 가능 반경 설정-좌표로 받아오기.
	HRESULT Init();
	void Release();
	void Update();
	void Render(HDC hdc);

	bool CanSee();
	bool Act(bool enemyInFov, bool justAlerted);		// execution, leaf
//이 act에서 두 bool값으로 어떤 행동을 취할지 결정(시야 안/인식)
	~AI();
};

class Acting
{
public:
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update() ;
	virtual void Render(HDC hdc);

	Acting() {};
	virtual bool act(string &status) = 0;
	virtual ~Acting() {};
};


class Hunting: public Acting
{
	// 이미지 애니메이션?'
	// 타게을 따라간다
	// 타겟값은 맵에서 받아온다.
	
public:
	using Super = Acting;

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	Hunting() {};
	virtual bool act(string& status) override;

	bool CanAttack();
	void Follow();	// 도착지가 플레이어의 위치
	void Attack();	// 플레이어가 이동 칸 안에 들어옴.

	virtual ~Hunting() {};
};

class Wandering: public Acting
{
	// 주변에 타겟 없음
	// 돌아다닌다
	// 내비게이션 필요?
	// 영역 정해서 그 중 아무데나 이동.(그냥 땅일때만 이동)
public:
	using Super = Acting;

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render(HDC hdc) override;

	Wandering() {};
	virtual bool act(string& status) override;

	bool RandomDestination();
	void Move();
	void Sleep();
	virtual ~Wandering() {};
};

// Status로 상태 받아옴-failure, succeeded, running
// failure이면 종료, succeeded면 끝난거니까 조건 확인, running은 기존ㄱ거 반보.