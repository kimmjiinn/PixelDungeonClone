#include "DungeonScene.h"
#include "DungeonTilemap.h"
#include "Player.h"
#include "Camera.h"
#include "TurnManager.h"
#include "KeyManager.h"

DungeonScene::DungeonScene()
    : tilemap(nullptr)
    , player(nullptr)
    , isInitialized(false)
{
}

DungeonScene::~DungeonScene()
{
    Release();
}

HRESULT DungeonScene::Init()
{
    // 타일맵 생성 및 초기화
    tilemap = new DungeonTilemap();
    if (FAILED(tilemap->Init()))
    {
        MessageBox(g_hWnd, TEXT("타일맵 초기화 실패"), TEXT("경고"), MB_OK);
        return E_FAIL;
    }
    
    // 플레이어 생성 및 초기화
    player = new Player({0,0});
    if (FAILED(player->Init()))
    {
        MessageBox(g_hWnd, TEXT("플레이어 초기화 실패"), TEXT("경고"), MB_OK);
        return E_FAIL;
    }
    
    // 카메라 초기화
    Camera::GetInstance()->Init();
    
    // 턴 매니저 초기화
    // TurnManager::GetInstance()->Init();
    
    // 플레이어에 타일맵 참조 설정
    // player->SetTilemap(tilemap);
    
    // 카메라에 타일맵과 플레이어 참조 설정
    Camera::GetInstance()->SetTilemap(tilemap);
    Camera::GetInstance()->SetPlayer(player);
    
    // 턴 매니저에 타일맵 참조 설정
    // TurnManager::GetInstance()->SetTilemap(tilemap);
    
    // 턴 매니저에 플레이어 추가
    // TurnManager::GetInstance()->AddActor(player);
    
    // 에너지 기반 턴 시스템 시작
    // 턴 매니저가 자동으로 액터들의 에너지를 관리하고 턴을 처리함
    
    isInitialized = true;
    return S_OK;
}

void DungeonScene::Release()
{
    // 플레이어 해제
    if (player)
    {
        player->Release();
        delete player;
        player = nullptr;
    }
    
    // 타일맵 해제
    if (tilemap)
    {
        tilemap->Release();
        delete tilemap;
        tilemap = nullptr;
    }
    
    // 카메라 해제
    Camera::GetInstance()->Release();
    
    // 턴 매니저 해제
    // TurnManager::GetInstance()->Release();
}

void DungeonScene::Update()
{
    if (!isInitialized) return;
    
    // 타일맵 업데이트
    if (tilemap) tilemap->Update();
    
    // 플레이어 업데이트
    if (player) player->Update();
    
    // 카메라 업데이트
    Camera::GetInstance()->Update();
    
    // 턴 매니저 업데이트 (에너지 기반 턴 처리)
    // TurnManager::GetInstance()->Update();
    
    // 카메라 플레이어 추적 토글 (F 키)
    if (KeyManager::GetInstance()->IsOnceKeyDown('F'))
    {
        Camera::GetInstance()->SetFollowPlayer(!Camera::GetInstance()->IsFollowingPlayer());
    }
    
    // 디버그 정보 표시 토글 (Tab 키)
    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_TAB))
    {
        // 디버그 정보 표시 기능은 필요시 구현
    }
}

void DungeonScene::Render(HDC hdc)
{
    if (!isInitialized) return;
    
    // 배경 지우기
    PatBlt(hdc, 0, 0, WINSIZE_X, WINSIZE_Y, WHITENESS);
    
    // 타일맵 렌더링
    if (tilemap) tilemap->Render(hdc);
    
    // 플레이어 렌더링
    if (player) player->Render(hdc);
    
    // 디버그 정보 표시
    wchar_t szText[128];
    wsprintf(szText, TEXT("FPS: %.1f"), 
        TimerManager::GetInstance()->GetDeltaTime());
        // TurnManager::GetInstance()->GetTurnCount());
    TextOut(hdc, 20, 20, szText, wcslen(szText));
    
    wsprintf(szText, TEXT("Camera: %.1f"), //, Follow: %s"), 
        Camera::GetInstance()->GetZoom());
        // Camera::GetInstance()->IsFollowingPlayer() ? L"ON" : L"OFF");
    TextOut(hdc, 20, 40, szText, wcslen(szText));
    
    if (player)
    {
        POINT pos = player->GetPos();
        wsprintf(szText, TEXT("Player: %d, %d, Energy: %d, Speed: %d"), 
            pos.x, pos.y,
            player->GetInfo().energy,
            player->GetInfo().speed);
        TextOut(hdc, 20, 60, szText, wcslen(szText));
    }
}
