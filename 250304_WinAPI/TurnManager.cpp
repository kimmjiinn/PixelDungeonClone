#include "TurnManager.h"
#include "DungeonTilemap.h"

TurnManager::TurnManager()
    : turnCount(0)
    , tilemap(nullptr)
    , isProcessingTurn(false)
{
}

TurnManager::~TurnManager()
{
    Release();
}

HRESULT TurnManager::Init()
{
    // 초기 설정
    turnCount = 0;
    isProcessingTurn = false;
    
    // 스케줄 큐 초기화 (이미 생성자에서 초기화됨)
    
    return S_OK;
}

void TurnManager::Release()
{
    // 스케줄 큐 비우기
    while (!schedule.empty())
    {
        schedule.pop();
    }
    
    // 참조만 하므로 해제할 것 없음
    tilemap = nullptr;
}

void TurnManager::Update()
{
    // 턴 처리 중이 아니면 리턴
    // if (!isProcessingTurn) return;
    
    // 턴 처리
    ProcessTurns();
}

void TurnManager::AddActor(Actor* actor)
{
    if (actor)
    {
        schedule.push(actor);
    }
}

void TurnManager::ProcessTurns()
{
    // 스케줄이 비어있으면 리턴
    if (schedule.empty()) return;
    
    // 현재 액터 가져오기
    Actor* current = schedule.top();
    schedule.pop();
    
    // 액터의 속도에 따라 에너지 추가
    current->energy += current->speed;
    
    // 에너지가 행동 비용보다 크거나 같으면 턴 실행
    if (current->energy >= Actor::MOVE_COST)
    {
        current->TakeTurn();
        
        // 액터가 내부적으로 에너지 비용을 차감함
    }
    
    // 액터가 아직 활성 상태면 다시 스케줄에 추가
    if (current->energy > 0)
    {
        schedule.push(current);
    }
    
    // 턴 카운트 증가
    turnCount++;
}

void TurnManager::SetTilemap(DungeonTilemap* tilemap)
{
    this->tilemap = tilemap;
}
