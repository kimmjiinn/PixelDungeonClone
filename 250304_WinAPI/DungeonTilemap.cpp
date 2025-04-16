#include "DungeonTilemap.h"
#include "Image.h"
#include "CommonFunction.h"
#include "KeyManager.h"

DungeonTilemap::DungeonTilemap()
    : tileImage(nullptr)
    , tiles(nullptr)
    , mapWidth(0)
    , mapHeight(0)
    , cameraZoom(1.0f)
    , isInitialized(false)
{
    cameraPos = { 0, 0 };
    selectedTile = { -1, -1 };
    playerPos = { 0, 0 };
}

DungeonTilemap::~DungeonTilemap()
{
    Release();
}

HRESULT DungeonTilemap::Init()
{
    // 기본 타일 이미지 로드
    tileImage = ImageManager::GetInstance()->AddImage(
        "배틀시티_샘플타일", L"Image/mapTiles.bmp", 640, 288,
        SAMPLE_TILE_X, SAMPLE_TILE_Y);
    
    if (!tileImage)
    {
        MessageBox(g_hWnd, TEXT("타일 이미지 로드 실패"), TEXT("경고"), MB_OK);
        return E_FAIL;
    }

    // 기본 맵 크기로 초기화
    InitTilemap(TILE_X, TILE_Y);

    return S_OK;
}

HRESULT DungeonTilemap::InitTilemap(int width, int height)
{
    // 기존 타일 정보 해제
    if (tiles)
    {
        delete[] tiles;
        tiles = nullptr;
    }

    // 맵 크기 설정
    mapWidth = width;
    mapHeight = height;

    // 타일 정보 배열 생성
    tiles = new DUNGEON_TILE_INFO[mapWidth * mapHeight];

    // 타일 정보 초기화
    for (int y = 0; y < mapHeight; ++y)
    {
        for (int x = 0; x < mapWidth; ++x)
        {
            int index = y * mapWidth + x;
            
            // 기본 바닥 타일로 초기화
            tiles[index].frameX = 0;
            tiles[index].frameY = 0;
            tiles[index].type = TileType::FLOOR;
            tiles[index].isVisible = false;
            tiles[index].isVisited = false;
            
            // 타일 영역 설정
            tiles[index].rc.left = x * TILE_SIZE;
            tiles[index].rc.top = y * TILE_SIZE;
            tiles[index].rc.right = tiles[index].rc.left + TILE_SIZE;
            tiles[index].rc.bottom = tiles[index].rc.top + TILE_SIZE;
        }
    }

    // 테스트용 맵 생성 (가장자리는 벽, 나머지는 바닥)
    for (int y = 0; y < mapHeight; ++y)
    {
        for (int x = 0; x < mapWidth; ++x)
        {
            if (x == 0 || y == 0 || x == mapWidth - 1 || y == mapHeight - 1)
            {
                // 가장자리는 벽으로 설정
                SetTile(x, y, 1, 0, TileType::WALL);
            }
            else
            {
                // 나머지는 바닥으로 설정
                SetTile(x, y, 3, 0, TileType::FLOOR);
            }
        }
    }

    // 테스트용 문과 계단 추가
    SetTile(mapWidth / 2, 0, 2, 0, TileType::DOOR);
    SetTile(mapWidth / 4, mapHeight / 2, 3, 0, TileType::STAIRS_DOWN);
    SetTile(mapWidth * 3 / 4, mapHeight / 2, 4, 0, TileType::STAIRS_UP);

    // 플레이어 초기 위치 설정 (중앙)
    playerPos.x = mapWidth / 2;
    playerPos.y = mapHeight / 2;

    // 카메라 초기 위치 설정 (플레이어 위치)
    cameraPos = playerPos;

    // 플레이어 주변 타일 가시성 설정
    UpdateVisibility();

    isInitialized = true;
    return S_OK;
}

void DungeonTilemap::Release()
{
    if (tiles)
    {
        delete[] tiles;
        tiles = nullptr;
    }
}

void DungeonTilemap::Update()
{
    if (!isInitialized) return;

    // 마우스 입력 처리
    if (PtInRect(&rcMain, g_ptMouse))
    {
        // 화면 좌표를 월드 좌표로 변환
        POINT worldPos = ScreenToWorld(g_ptMouse.x, g_ptMouse.y);
        
        // 맵 범위 내인지 확인
        if (worldPos.x >= 0 && worldPos.x < mapWidth && 
            worldPos.y >= 0 && worldPos.y < mapHeight)
        {
            // 선택된 타일 업데이트
            selectedTile = worldPos;
            
            // 마우스 왼쪽 버튼 클릭 시 타일 선택
            if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
            {
                // 선택된 타일이 이동 가능한지 확인
                if (IsWalkable(selectedTile.x, selectedTile.y))
                {
                    // 플레이어 이동 처리는 Player 클래스에서 구현
                }
            }
        }
    }

    // 카메라 줌 처리
    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_ADD) || 
        KeyManager::GetInstance()->IsOnceKeyDown(VK_OEM_PLUS))
    {
        ChangeZoom(0.1f);
    }
    else if (KeyManager::GetInstance()->IsOnceKeyDown(VK_SUBTRACT) || 
             KeyManager::GetInstance()->IsOnceKeyDown(VK_OEM_MINUS))
    {
        ChangeZoom(-0.1f);
    }

    // 카메라 이동 처리
    if (KeyManager::GetInstance()->IsStayKeyDown(VK_LEFT))
    {
        MoveCamera(-1, 0);
    }
    else if (KeyManager::GetInstance()->IsStayKeyDown(VK_RIGHT))
    {
        MoveCamera(1, 0);
    }
    
    if (KeyManager::GetInstance()->IsStayKeyDown(VK_UP))
    {
        MoveCamera(0, -1);
    }
    else if (KeyManager::GetInstance()->IsStayKeyDown(VK_DOWN))
    {
        MoveCamera(0, 1);
    }

    // 마우스 오른쪽 버튼 드래그로 카메라 이동
    static POINT prevMousePos = { 0, 0 };
    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RBUTTON))
    {
        prevMousePos = g_ptMouse;
    }
    else if (KeyManager::GetInstance()->IsStayKeyDown(VK_RBUTTON))
    {
        int deltaX = (prevMousePos.x - g_ptMouse.x) / (TILE_SIZE * cameraZoom);
        int deltaY = (prevMousePos.y - g_ptMouse.y) / (TILE_SIZE * cameraZoom);
        
        if (deltaX != 0 || deltaY != 0)
        {
            MoveCamera(deltaX, deltaY);
            prevMousePos = g_ptMouse;
        }
    }
}

void DungeonTilemap::Render(HDC hdc)
{
    if (!isInitialized || !tileImage) return;

    // 메인 영역 설정
    rcMain.left = 0;
    rcMain.top = 0;
    rcMain.right = WINSIZE_X;
    rcMain.bottom = WINSIZE_Y;

    // 화면에 보이는 타일 범위 계산
    int tileSize = (int)(TILE_SIZE * cameraZoom);
    int startX = cameraPos.x - (WINSIZE_X / 2) / tileSize;
    int startY = cameraPos.y - (WINSIZE_Y / 2) / tileSize;
    int endX = cameraPos.x + (WINSIZE_X / 2) / tileSize + 1;
    int endY = cameraPos.y + (WINSIZE_Y / 2) / tileSize + 1;

    // 맵 범위를 벗어나지 않도록 조정
    startX = max(0, startX);
    startY = max(0, startY);
    endX = min(mapWidth, endX);
    endY = min(mapHeight, endY);

    // 타일 렌더링
    for (int y = startY; y < endY; ++y)
    {
        for (int x = startX; x < endX; ++x)
        {
            int index = y * mapWidth + x;
            
            // 타일이 보이지 않는 경우 검은색으로 표시
            if (!tiles[index].isVisible && !tiles[index].isVisited)
            {
                continue;
            }

            // 방문했지만 현재 보이지 않는 타일은 어둡게 표시
            bool isDim = tiles[index].isVisited && !tiles[index].isVisible;
            
            // 화면 좌표 계산
            POINT screenPos = WorldToScreen(x, y);
            
            // 타일 렌더링
            tileImage->FrameRender(
                hdc, 
                screenPos.x, 
                screenPos.y, 
                tiles[index].frameX, 
                tiles[index].frameY
            );
            
            // 선택된 타일 표시
            if (x == selectedTile.x && y == selectedTile.y)
            {
                HPEN hPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));
                HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
                
                HBRUSH hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
                HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

                RenderRectAtCenter(hdc, 
                    screenPos.x, 
                    screenPos.y,tileSize, tileSize);
                // Rectangle(hdc, 
                //     screenPos.x, 
                //     screenPos.y, 
                //     screenPos.x + tileSize, 
                //     screenPos.y + tileSize);
                
                SelectObject(hdc, hOldPen);
                SelectObject(hdc, hOldBrush);
                DeleteObject(hPen);
            }
            
            // 플레이어 위치 표시
            if (x == playerPos.x && y == playerPos.y)
            {
                // 플레이어 아이콘 렌더링 (임시로 빨간색 원으로 표시)
                HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
                HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
                
                Ellipse(hdc, 
                    screenPos.x + tileSize / 4, 
                    screenPos.y + tileSize / 4, 
                    screenPos.x + tileSize * 3 / 4, 
                    screenPos.y + tileSize * 3 / 4);
                
                SelectObject(hdc, hOldBrush);
                DeleteObject(hBrush);
            }
        }
    }

    // 디버그 정보 표시
    wchar_t szText[128];
    wsprintf(szText, TEXT("Zoom: %.1f, Camera: %d, %d, Player: %d, %d"), 
        cameraZoom, cameraPos.x, cameraPos.y, playerPos.x, playerPos.y);
    TextOut(hdc, 20, 120, szText, wcslen(szText));
    
    if (selectedTile.x >= 0 && selectedTile.y >= 0)
    {
        wsprintf(szText, TEXT("Selected: %d, %d, Type: %d"), 
            selectedTile.x, selectedTile.y, 
            (int)GetTileType(selectedTile.x, selectedTile.y));
        TextOut(hdc, 20, 140, szText, wcslen(szText));
    }
}

void DungeonTilemap::SetTile(int x, int y, int frameX, int frameY, TileType type)
{
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
        return;

    int index = y * mapWidth + x;
    tiles[index].frameX = frameX;
    tiles[index].frameY = frameY;
    tiles[index].type = type;
}

DUNGEON_TILE_INFO DungeonTilemap::GetTile(int x, int y)
{
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
    {
        DUNGEON_TILE_INFO emptyTile = { 0 };
        emptyTile.type = TileType::WALL;
        return emptyTile;
    }

    int index = y * mapWidth + x;
    return tiles[index];
}

TileType DungeonTilemap::GetTileType(int x, int y)
{
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
        return TileType::WALL;

    int index = y * mapWidth + x;
    return tiles[index].type;
}

bool DungeonTilemap::IsWalkable(int x, int y)
{
    TileType type = GetTileType(x, y);
    return (type == TileType::FLOOR || type == TileType::DOOR || 
            type == TileType::STAIRS_DOWN || type == TileType::STAIRS_UP);
}

void DungeonTilemap::SetTileVisibility(int x, int y, bool isVisible)
{
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
        return;

    int index = y * mapWidth + x;
    tiles[index].isVisible = isVisible;
    
    if (isVisible)
        tiles[index].isVisited = true;
}

void DungeonTilemap::SetTileVisited(int x, int y, bool isVisited)
{
    if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
        return;

    int index = y * mapWidth + x;
    tiles[index].isVisited = isVisited;
}

void DungeonTilemap::SetCameraPos(int x, int y)
{
    // 맵 범위를 벗어나지 않도록 조정
    cameraPos.x = max(0, min(mapWidth - 1, x));
    cameraPos.y = max(0, min(mapHeight - 1, y));
}

void DungeonTilemap::MoveCamera(int deltaX, int deltaY)
{
    SetCameraPos(cameraPos.x + deltaX, cameraPos.y + deltaY);
}

void DungeonTilemap::SetCameraZoom(float zoom)
{
    // 줌 레벨 제한 (0.5 ~ 2.0)
    cameraZoom = max(0.5f, min(2.0f, zoom));
}

void DungeonTilemap::ChangeZoom(float deltaZoom)
{
    SetCameraZoom(cameraZoom + deltaZoom);
}

POINT DungeonTilemap::WorldToScreen(int worldX, int worldY)
{
    POINT screenPos;
    int tileSize = (int)(TILE_SIZE * cameraZoom);
    
    screenPos.x = (worldX - cameraPos.x) * tileSize + WINSIZE_X / 2;
    screenPos.y = (worldY - cameraPos.y) * tileSize + WINSIZE_Y / 2;
    
    return screenPos;
}

POINT DungeonTilemap::ScreenToWorld(int screenX, int screenY)
{
    POINT worldPos;
    int tileSize = (int)(TILE_SIZE * cameraZoom);
    
    worldPos.x = (screenX - WINSIZE_X / 2) / tileSize + cameraPos.x;
    worldPos.y = (screenY - WINSIZE_Y / 2) / tileSize + cameraPos.y;
    
    return worldPos;
}

void DungeonTilemap::SaveMap(const wchar_t* fileName)
{
    // 파일 저장
    HANDLE hFile = CreateFile(
        fileName, GENERIC_WRITE, 0, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (hFile == INVALID_HANDLE_VALUE)
    {
        MessageBox(g_hWnd, TEXT("맵 저장 실패"), TEXT("경고"), MB_OK);
        return;
    }
    
    // 맵 크기 저장
    DWORD dwByte = 0;
    WriteFile(hFile, &mapWidth, sizeof(int), &dwByte, NULL);
    WriteFile(hFile, &mapHeight, sizeof(int), &dwByte, NULL);
    
    // 타일 정보 저장
    WriteFile(hFile, tiles, sizeof(DUNGEON_TILE_INFO) * mapWidth * mapHeight, &dwByte, NULL);
    
    CloseHandle(hFile);
}

void DungeonTilemap::LoadMap(const wchar_t* fileName)
{
    // 파일 로드
    HANDLE hFile = CreateFile(
        fileName, GENERIC_READ, 0, NULL,
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (hFile == INVALID_HANDLE_VALUE)
    {
        MessageBox(g_hWnd, TEXT("맵 로드 실패"), TEXT("경고"), MB_OK);
        return;
    }
    
    // 기존 타일 정보 해제
    if (tiles)
    {
        delete[] tiles;
        tiles = nullptr;
    }
    
    // 맵 크기 로드
    DWORD dwByte = 0;
    ReadFile(hFile, &mapWidth, sizeof(int), &dwByte, NULL);
    ReadFile(hFile, &mapHeight, sizeof(int), &dwByte, NULL);
    
    // 타일 정보 배열 생성
    tiles = new DUNGEON_TILE_INFO[mapWidth * mapHeight];
    
    // 타일 정보 로드
    ReadFile(hFile, tiles, sizeof(DUNGEON_TILE_INFO) * mapWidth * mapHeight, &dwByte, NULL);
    
    CloseHandle(hFile);
    
    // 플레이어 위치 초기화
    playerPos.x = mapWidth / 2;
    playerPos.y = mapHeight / 2;
    
    // 카메라 위치 초기화
    cameraPos = playerPos;
    
    isInitialized = true;
}

void DungeonTilemap::SetPlayerPos(int x, int y)
{
    // 맵 범위를 벗어나지 않도록 조정
    playerPos.x = max(0, min(mapWidth - 1, x));
    playerPos.y = max(0, min(mapHeight - 1, y));
    
    // 플레이어 주변 타일 가시성 업데이트
    UpdateVisibility();
}

void DungeonTilemap::SetSelectedTile(int x, int y)
{
    selectedTile.x = x;
    selectedTile.y = y;
}

// 플레이어 주변 타일 가시성 업데이트
void DungeonTilemap::UpdateVisibility()
{
    // 시야 범위 (플레이어 주변 5칸)
    const int viewRange = 5;
    
    for (int y = playerPos.y - viewRange; y <= playerPos.y + viewRange; ++y)
    {
        for (int x = playerPos.x - viewRange; x <= playerPos.x + viewRange; ++x)
        {
            if (x >= 0 && x < mapWidth && y >= 0 && y < mapHeight)
            {
                // 플레이어와의 거리 계산
                int distance = abs(x - playerPos.x) + abs(y - playerPos.y);
                
                if (distance <= viewRange)
                {
                    SetTileVisibility(x, y, true);
                }
            }
        }
    }
}
