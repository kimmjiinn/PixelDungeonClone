#pragma once
#include "config.h"
#include <vector>
#include <queue>
#include <functional>

#include "GameObject.h"

class DungeonTilemap;

/**
 * @class Actor
 * @brief 게임 내 모든 행동 가능한 개체의 기본 클래스
 * 
 * 에너지 기반 턴 시스템을 위한 기본 클래스입니다.
 * 각 액터는 속도에 따라 에너지를 축적하고, 행동에 따라 에너지를 소모합니다.
 */
class Actor : public GameObject
{
public:
    int energy{0};               // 현재 에너지
    int speed{10};               // 속도 (높을수록 빠름)
    
    // 행동 에너지 비용
    static constexpr int MOVE_COST = 100;       // 이동 비용
    static constexpr int ATTACK_COST = 80;      // 공격 비용
    static constexpr int USE_ITEM_COST = 120;   // 아이템 사용 비용
    
    /**
     * @brief 턴 실행 (각 액터가 구현해야 함)
     */
    virtual void TakeTurn() = 0;
    
    /**
     * @brief 액터 초기화
     * @return 초기화 결과
     */
    virtual HRESULT Init() override { return S_OK; }
    
    /**
     * @brief 액터 메모리 해제
     */
    virtual void Release() override {}
    
    /**
     * @brief 액터 업데이트
     */
    virtual void Update() override {}
    
    /**
     * @brief 액터 렌더링
     * @param hdc 렌더링할 DC
     */
    virtual void Render(HDC hdc) override {}
    
    Actor() {}
    virtual ~Actor() {}
};

/**
 * @struct ActorCompare
 * @brief 액터 우선순위 비교 함수 객체
 * 
 * 에너지가 낮은 액터가 우선순위가 높습니다.
 */
struct ActorCompare
{
    bool operator()(const Actor* a, const Actor* b) const
    {
        return a->energy > b->energy;
    }
};

/**
 * @class TurnManager
 * @brief 에너지 기반 턴 관리 시스템
 * 
 * 액터들의 에너지를 관리하고 턴을 처리합니다.
 */
class TurnManager : public Singleton<TurnManager>
{
public:
    using ScheduleQueue = std::priority_queue<Actor*, std::vector<Actor*>, ActorCompare>;
    
private:
    ScheduleQueue schedule;      // 액터 스케줄 큐
    int turnCount;               // 턴 카운트
    DungeonTilemap* tilemap;     // 타일맵 참조
    bool isProcessingTurn;       // 턴 처리 중인지 여부

public:
    /**
     * @brief 턴 매니저 초기화
     * @return 초기화 결과
     */
    HRESULT Init();
    
    /**
     * @brief 턴 매니저 메모리 해제
     */
    void Release();
    
    /**
     * @brief 턴 매니저 업데이트
     */
    void Update();
    
    /**
     * @brief 액터 추가
     * @param actor 추가할 액터
     */
    void AddActor(Actor* actor);
    
    /**
     * @brief 턴 처리
     */
    void ProcessTurns();
    
    /**
     * @brief 타일맵 설정
     * @param tilemap 타일맵 포인터
     */
    void SetTilemap(DungeonTilemap* tilemap);
    
    /**
     * @brief 턴 카운트 가져오기
     * @return 턴 카운트
     */
    int GetTurnCount() { return turnCount; }
    
    /**
     * @brief 턴 처리 중인지 확인
     * @return 턴 처리 중 여부
     */
    bool IsProcessingTurn() { return isProcessingTurn; }

    TurnManager();
    ~TurnManager();
};
