#pragma once
#include "GameObject.h"
#include "TileMap.h"
#include "Camera.h"
#include "PathFinder.h"
#include <vector>

class Player;
class Enemy;
class TurnManager;

// AstarScene 클래스: 게임의 메인 씬 클래스
class AstarScene : public GameObject
{
public:
	virtual ~AstarScene() {};
    
	virtual HRESULT Init();
	virtual void Release();
	virtual void Update();
	virtual void Render(HDC hdc);
    
	// 입력 처리
	void ProcessInput();
    
	// 턴 기반 시스템
	void InitializeTurnSystem();
	void ProcessMovementTurn();
    
	// 마우스 입력 관련 함수
	int GetMouseWheelDelta();
	bool IsMouseDragging();
	POINT GetMouseDragDelta();

	// AI
	void Act();
	bool CanSee();
	void Hunting();
	bool CanAttack();
	void Follow();
	void Wandering();
	void SetEnemyFov();		// 우선은 Astar에서 설정
	AstarTile* RandomDestination();
	vector<AstarTile*> enemyFovList;
	bool isAttack = false;

private:
	// 컴포넌트 클래스들
	TileMap* tileMap;       // 타일맵 관리
	Camera* camera;         // 카메라 관리
	PathFinder* pathFinder; // 경로 탐색
    //
	PathFinder* enemyPathFinder; // 적의 경로 탐색


	// 게임 오브젝트
	Player* player;         // 플레이어
	Enemy* enemy;           // 적
    
	// 이동 관련 변수
	bool moving = false;            // 이동 중 여부
	float moveTimer = 0.0f;         // 이동 타이머
	std::vector<POINT> path;        // 이동 경로
	int pathIdx = 0;                // 현재 경로 인덱스

	// enemy 이동 관련 변수
	bool enemyMoving = false;
	float enemyMoveTimer = 0.0f;         // 이동 타이머
	vector<POINT> enemyPath;
	int enemyPathIdx = 0;
    
	// 턴 기반 시스템 관련 변수
	bool turnInitialized = false;   // 턴 시스템 초기화 여부
};
