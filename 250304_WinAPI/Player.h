#pragma once
#include "TurnManager.h"
#include "config.h"
#include <vector>


class Image;
class DungeonTilemap;

// 플레이어 상태
enum class PlayerState
{
    IDLE,       // 대기 상태
    MOVING,     // 이동 중
    ATTACKING,  // 공격 중
    USING_ITEM  // 아이템 사용 중
};

/**
 * @class Player
 * @brief 플레이어 캐릭터 클래스
 * 
 * Actor를 상속받아 에너지 기반 턴 시스템에 통합됩니다.
 */
class Player : public Actor
{
private:
    Image* playerImage;          // 플레이어 이미지
    DungeonTilemap* tilemap;     // 타일맵 참조
    
    POINT position;              // 현재 위치 (타일 단위)
    POINT targetPosition;        // 목표 위치 (타일 단위)
    std::vector<POINT> path;     // 이동 경로
    
    PlayerState state;           // 현재 상태
    int currentPathIndex;        // 현재 경로 인덱스
    
    int frameX;                  // 현재 프레임 X
    int frameY;                  // 현재 프레임 Y
    
    float moveTimer;             // 이동 타이머
    float moveDelay;             // 이동 지연 시간 (턴 속도)
    
    bool isMoving;               // 이동 중인지 여부
    bool isTurnEnded;            // 턴이 끝났는지 여부

public:
    /**
     * @brief 플레이어 초기화
     * @return 초기화 결과
     */
    virtual HRESULT Init() override;
    
    /**
     * @brief 플레이어 메모리 해제
     */
    virtual void Release() override;
    
    /**
     * @brief 플레이어 업데이트
     */
    virtual void Update() override;
    
    /**
     * @brief 플레이어 렌더링
     * @param hdc 렌더링할 DC
     */
    virtual void Render(HDC hdc) override;
    
    /**
     * @brief 턴 실행 (Actor 클래스의 가상 함수 구현)
     */
    virtual void TakeTurn() override;
    
    /**
     * @brief 타일맵 설정
     * @param tilemap 타일맵 포인터
     */
    void SetTilemap(DungeonTilemap* tilemap);
    
    /**
     * @brief 위치 설정
     * @param x X 좌표
     * @param y Y 좌표
     */
    void SetPosition(int x, int y);
    
    /**
     * @brief 위치 가져오기
     * @return 플레이어 위치
     */
    POINT GetPosition() { return position; }
    
    /**
     * @brief 목표 위치로 이동 명령
     * @param x 목표 X 좌표
     * @param y 목표 Y 좌표
     */
    void MoveTo(int x, int y);
    
    /**
     * @brief 경로 찾기 (A* 알고리즘)
     * @param start 시작 위치
     * @param end 목표 위치
     * @return 찾은 경로
     */
    std::vector<POINT> FindPath(POINT start, POINT end);
    
    /**
     * @brief 다음 타일로 이동
     */
    void MoveToNextTile();
    
    /**
     * @brief 턴이 끝났는지 확인
     * @return 턴 종료 여부
     */
    bool IsTurnEnded() { return isTurnEnded; }
    
    /**
     * @brief 이동 중인지 확인
     * @return 이동 중 여부
     */
    bool IsMoving() { return isMoving; }
    
    /**
     * @brief 상태 설정
     * @param newState 새 상태
     */
    void SetState(PlayerState newState);
    
    /**
     * @brief 상태 가져오기
     * @return 현재 상태
     */
    PlayerState GetState() { return state; }
    
    /**
     * @brief 입력 처리
     */
    void HandleInput();

    Player();
    virtual ~Player();
};
