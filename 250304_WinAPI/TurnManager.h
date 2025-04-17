#pragma once
#include "Singleton.h"
#include <vector>

#include "Enemy.h"

class Actor;

class TurnManager : public Singleton<TurnManager>
{
public:
	TurnManager();
	~TurnManager();

	// 액터 관리 함수
	void AddActor(Actor* actor);
	void RemoveActor(Actor* actor);
	void ClearActors();
	
	// 턴 처리 함수
	void ProcessTurn();
	
	// 턴 시스템 활성화/비활성화
	void SetActive(bool active) { isActive = active; }
	bool IsActive() const { return isActive; }
	
	// 턴 시작/종료
	void StartTurn() { isActive = true; }
	void EndTurn() { isActive = false; }

private:
	std::vector<Actor*> actors;
	bool isActive = false;
};