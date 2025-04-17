#include "AstarScene.h"
#include "Player.h"
#include "Enemy.h"
#include "TurnManager.h"
#include "KeyManager.h"
#include "TimerManager.h"
#include "SceneManager.h"
#include "config.h"

HRESULT AstarScene::Init()
{
    // 컴포넌트 클래스 생성 및 초기화
    tileMap = new TileMap();
    tileMap->Init();
    
    camera = new Camera();
    camera->Init(WINSIZE_X, WINSIZE_Y);
    
    pathFinder = new PathFinder();
    pathFinder->Init(tileMap);
    
    // 플레이어 생성 및 초기화
    AstarTile* startTile = tileMap->GetTile(3, 3);
    player = new Player({startTile->GetCenter().x, startTile->GetCenter().y});
    player->Init();
    
    // 적 생성 및 초기화
    AstarTile* enemyStartTile = tileMap->GetTile(18, 5);
    enemy = new Enemy({enemyStartTile->GetCenter().x, enemyStartTile->GetCenter().y});
    enemy->Init();
    
    return S_OK;
}

void AstarScene::Release()
{
    // // 메모리 해제
    // SAFE_DELETE(player);
    // SAFE_DELETE(enemy);
    // SAFE_DELETE(pathFinder);
    // SAFE_DELETE(camera);
    // SAFE_DELETE(tileMap);
}

void AstarScene::Update()
{
    float deltaTime = TimerManager::GetInstance()->GetDeltaTime();
    
    // 타일맵 업데이트
    tileMap->Update();
    
    // 입력 처리
    ProcessInput();
    
    // 턴 기반 시스템 초기화 (첫 프레임에만)
    if (!turnInitialized)
    {
        InitializeTurnSystem();
        turnInitialized = true;
    }
    
    // 플레이어 자동 이동 처리
    if (moving)
    {
        if (path.empty() || pathIdx >= path.size())
        {
            moving = false;
            return;
        }
        
        // 이동 타이머 업데이트
        moveTimer += deltaTime;
        if (moveTimer >= 0.2f) // 이동 속도
        {
            // 다음 타일로 이동
            POINT nextPos = path[pathIdx];
            player->SetPos(nextPos);
            
            // 이동 후 턴 소모 처리
            ProcessMovementTurn();
            
            // 타이머 리셋 및 다음 경로 인덱스로 이동
            moveTimer = 0;
            pathIdx++;
        }
    }
    
    // 플레이어 업데이트
    if (player)
    {
        player->Update();
        
        // 카메라 위치 업데이트 (플레이어 중심)
        camera->SetPosition(player->GetPos());
    }
    
    // 적 업데이트
    if (enemy)
    {
        // 플레이어 감지 로직
        if (!isTarget)
        {
            LookAround();
        }
        
        if (isTarget)
        {
            UpdateTargetPos();
        }
        
        enemy->Update();
    }
    
    // 카메라 업데이트
    camera->Update();
}

void AstarScene::Render(HDC hdc)
{
    // 카메라 변환 적용
    camera->SetViewport(hdc);
    
    // 타일맵 렌더링
    tileMap->Render(hdc);
    
    // 플레이어와 적 렌더링
    if (player)
        player->Render(hdc);
        
    if (enemy)
        enemy->Render(hdc);
    
    // 카메라 변환 해제 (UI 렌더링을 위해)
    camera->ResetViewport(hdc);
    
    // UI 렌더링
    wchar_t statusText[128];
    wsprintf(statusText, L"턴 기반 시스템: %s", TurnManager::GetInstance()->IsActive() ? L"활성화" : L"비활성화");
    TextOut(hdc, 10, 10, statusText, wcslen(statusText));
    
    wsprintf(statusText, L"카메라 줌: %.1f (마우스 휠로 조절)", camera->GetZoom());
    TextOut(hdc, 10, 30, statusText, wcslen(statusText));
    
    wsprintf(statusText, L"마우스 좌클릭: 이동, 우클릭: 벽 생성, 드래그: 카메라 이동");
    TextOut(hdc, 10, 50, statusText, wcslen(statusText));
}

void AstarScene::ProcessInput()
{
    // 마우스 휠로 줌 조절
    int mouseWheel = GetMouseWheelDelta();
    if (mouseWheel != 0)
    {
        camera->Zoom(mouseWheel * 0.1f);
    }
    
    // 마우스 드래그로 카메라 이동
    if (IsMouseDragging())
    {
        POINT dragDelta = GetMouseDragDelta();
        camera->Move(-dragDelta.x / camera->GetZoom(), -dragDelta.y / camera->GetZoom());
    }
    
    // 마우스 우클릭으로 벽 생성
    if (KeyManager::GetInstance()->IsStayKeyDown(VK_RBUTTON))
    {
        // 화면 좌표를 월드 좌표로 변환
        POINT screenPos = { g_ptMouse.x, g_ptMouse.y };
        POINT worldPos = camera->ScreenToWorld(screenPos);
        
        // 월드 좌표를 타일 인덱스로 변환
        int tileX = worldPos.x / TILE_SIZE;
        int tileY = worldPos.y / TILE_SIZE;
        
        // 타일 타입 변경
        if (tileMap->GetTile(tileX, tileY))
        {
            tileMap->SetTileType(tileX, tileY, AstarTile::TileType::Wall);
        }
    }
    
    // 마우스 좌클릭으로 목적지 설정 및 자동 이동
    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
    {
        // 화면 좌표를 월드 좌표로 변환
        POINT screenPos = { g_ptMouse.x, g_ptMouse.y };
        POINT worldPos = camera->ScreenToWorld(screenPos);
        
        // 월드 좌표를 타일 인덱스로 변환
        int tileX = worldPos.x / TILE_SIZE;
        int tileY = worldPos.y / TILE_SIZE;
        
        // 목적지 타일 설정
        AstarTile* clickTile = tileMap->GetTile(tileX, tileY);
        if (clickTile && clickTile->GetType() != AstarTile::TileType::Wall)
        {
            // 목적지 설정
            tileMap->SetDestTile(tileX, tileY);
            
            // 현재 플레이어 위치 타일 찾기
            POINT playerPos = player->GetPos();
            AstarTile* playerTile = tileMap->GetTileFromPos(playerPos.x, playerPos.y);
            
            // 경로 찾기
            path = pathFinder->FindPath(playerTile, clickTile);
            
            // 경로가 있으면 이동 시작
            if (!path.empty())
            {
                moving = true;
                pathIdx = 0;
                moveTimer = 0;
                
                // 턴 기반 시스템 활성화
                TurnManager::GetInstance()->StartTurn();
            }
        }
    }
    
    // 턴 시작/종료 키
    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SPACE))
    {
        if (!TurnManager::GetInstance()->IsActive())
        {
            TurnManager::GetInstance()->StartTurn();
        }
        else
        {
            TurnManager::GetInstance()->EndTurn();
        }
    }
    
    // 씬 전환
    if (KeyManager::GetInstance()->IsStayKeyDown(VK_RETURN))
    {
        SceneManager::GetInstance()->ChangeScene("전투씬_1");
    }
}

void AstarScene::InitializeTurnSystem()
{
    // TurnManager 초기화
    TurnManager::GetInstance()->ClearActors();
    
    // 플레이어와 적을 액터로 등록
    TurnManager::GetInstance()->AddActor(player);
    TurnManager::GetInstance()->AddActor(enemy);
    
    // 턴 시스템 비활성화 상태로 시작
    TurnManager::GetInstance()->SetActive(false);
}

void AstarScene::ProcessMovementTurn()
{
    // 플레이어 이동 시 턴 처리
    // 플레이어가 한 타일 이동할 때마다 턴이 소모되고 적이 이동함
    
    // 적 AI 업데이트 (플레이어 이동 후)
    if (enemy && isTarget)
    {
        // 적의 다음 위치 계산
        UpdateTargetPos();
        
        // 플레이어 위치 타일 찾기
        POINT playerPos = player->GetPos();
        AstarTile* playerTile = tileMap->GetTileFromPos(playerPos.x, playerPos.y);
        
        // 적 위치 타일 찾기
        POINT enemyPos = enemy->GetPos();
        AstarTile* enemyTile = tileMap->GetTileFromPos(enemyPos.x, enemyPos.y);
        
        // 경로 찾기
        std::vector<POINT> enemyPath = pathFinder->FindPath(enemyTile, playerTile);
        
        // 적 이동 (첫 번째 경로 지점으로)
        if (!enemyPath.empty())
        {
            enemy->SetPos(enemyPath[0]);
        }
    }
}

int AstarScene::GetMouseWheelDelta()
{
    // 마우스 휠 델타 값 반환 (실제 구현에서는 윈도우 메시지 처리 필요)
    // 임시 구현: Q/E 키로 대체
    if (KeyManager::GetInstance()->IsOnceKeyDown('Q'))
        return -1;
    else if (KeyManager::GetInstance()->IsOnceKeyDown('E'))
        return 1;
    return 0;
}

bool AstarScene::IsMouseDragging()
{
    // 마우스 드래그 여부 반환 (실제 구현에서는 윈도우 메시지 처리 필요)
    // 임시 구현: 마우스 중간 버튼 클릭으로 대체
    return KeyManager::GetInstance()->IsStayKeyDown(VK_MBUTTON);
}

POINT AstarScene::GetMouseDragDelta()
{
    // 마우스 드래그 델타 값 반환 (실제 구현에서는 윈도우 메시지 처리 필요)
    // 임시 구현: 고정 값 반환
    POINT delta = { 0, 0 };
    if (KeyManager::GetInstance()->IsStayKeyDown(VK_LEFT))
        delta.x = -5;
    else if (KeyManager::GetInstance()->IsStayKeyDown(VK_RIGHT))
        delta.x = 5;
    
    if (KeyManager::GetInstance()->IsStayKeyDown(VK_UP))
        delta.y = -5;
    else if (KeyManager::GetInstance()->IsStayKeyDown(VK_DOWN))
        delta.y = 5;
    
    return delta;
}

void AstarScene::LookAround()
{
    // 적이 플레이어를 감지하는 로직
    static const int dx[8] = { -1, 1, -1, 1, -1, 1, 0, 0 };
    static const int dy[8] = { -1, 1, 1, -1, 0, 0, -1, 1 };
    
    // 적 위치 타일 찾기
    POINT enemyPos = enemy->GetPos();
    AstarTile* enemyTile = tileMap->GetTileFromPos(enemyPos.x, enemyPos.y);
    
    if (!enemyTile)
        return;
        
    // 주변 8방향 탐색
    for (int i = 0; i < 8; i++)
    {
        int nx = enemyTile->GetIdX() + dx[i];
        int ny = enemyTile->GetIdY() + dy[i];
        
        AstarTile* nearTile = tileMap->GetTile(nx, ny);
        if (!nearTile)
            continue;
            
        // 플레이어 위치 타일 찾기
        POINT playerPos = player->GetPos();
        AstarTile* playerTile = tileMap->GetTileFromPos(playerPos.x, playerPos.y);
        
        if (playerTile && nearTile == playerTile)
        {
            isTarget = true;
            break;
        }
    }
}

void AstarScene::UpdateTargetPos()
{
    // 적의 목표 위치 업데이트 (플레이어 위치)
    if (!isTarget || !player)
        return;
        
    // 플레이어 위치 타일 찾기
    POINT playerPos = player->GetPos();
    AstarTile* playerTile = tileMap->GetTileFromPos(playerPos.x, playerPos.y);
    
    if (playerTile)
    {
        // 적 위치 타일 찾기
        POINT enemyPos = enemy->GetPos();
        AstarTile* enemyTile = tileMap->GetTileFromPos(enemyPos.x, enemyPos.y);
        
        if (enemyTile)
        {
            // 경로 찾기
            std::vector<POINT> enemyPath = pathFinder->FindPath(enemyTile, playerTile);
            
            // 적 이동 (첫 번째 경로 지점으로)
            if (!enemyPath.empty())
            {
                enemy->SetPos(enemyPath[0]);
            }
        }
    }
}
