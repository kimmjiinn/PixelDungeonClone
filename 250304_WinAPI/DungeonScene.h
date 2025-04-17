#pragma once
#include "GameObject.h"
#include "config.h"

class DungeonTilemap;
class Player;
class Actor;

/**
 * @class DungeonScene
 * @brief 에너지 기반 턴 시스템을 사용하는 던전 게임 씬 클래스
 * 
 * 던전 게임의 메인 씬을 구현하며, 타일맵, 플레이어, 카메라 등의 시스템을 통합합니다.
 * 에너지 기반 턴 시스템을 사용합니다.
 */
class DungeonScene : public GameObject
{
private:
    DungeonTilemap* tilemap;     // 타일맵
    Player* player;              // 플레이어
    
    bool isInitialized;          // 초기화 여부

public:
    /**
     * @brief 던전 씬 초기화
     * @return 초기화 결과
     */
    virtual HRESULT Init() override;
    
    /**
     * @brief 던전 씬 메모리 해제
     */
    virtual void Release() override;
    
    /**
     * @brief 던전 씬 업데이트
     */
    virtual void Update() override;
    
    /**
     * @brief 던전 씬 렌더링
     * @param hdc 렌더링할 DC
     */
    virtual void Render(HDC hdc) override;
    
    DungeonScene();
    virtual ~DungeonScene();
};
