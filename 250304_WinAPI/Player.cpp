#include "Player.h"
#include "Image.h"
#include "DungeonTilemap.h"
#include "KeyManager.h"
#include <queue>
#include <map>
#include <cmath>

Player::Player()
    : playerImage(nullptr)
    , tilemap(nullptr)
    , state(PlayerState::IDLE)
    , currentPathIndex(0)
    , frameX(0)
    , frameY(0)
    , moveTimer(0.0f)
    , moveDelay(0.5f)  // 0.5초마다 한 칸 이동 (턴 속도)
    , isMoving(false)
    , isTurnEnded(true)
{
    position = { 0, 0 };
    targetPosition = { 0, 0 };
    
    // 에너지 시스템 초기화
    energy = 0;
    speed = 15;  // 플레이어 기본 속도
}

Player::~Player()
{
    Release();
}

HRESULT Player::Init()
{
    // 플레이어 이미지 로드
    playerImage = ImageManager::GetInstance()->AddImage(
        "픽셀던전_플레이어", L"Image/ufo.bmp", 530, 32,
        10, 1,true, RGB(255,0,255));

    if (!playerImage)
    {
        MessageBox(g_hWnd, TEXT("플레이어 이미지 로드 실패"), TEXT("경고"), MB_OK);
        return E_FAIL;
    }

    // 초기 상태 설정
    state = PlayerState::IDLE;
    frameX = 0;
    frameY = 0;
    
    // 초기 위치 설정 (타일맵 중앙)
    if (tilemap)
    {
        position.x = tilemap->GetMapWidth() / 2;
        position.y = tilemap->GetMapHeight() / 2;
        tilemap->SetPlayerPos(position.x, position.y);
    }

    return S_OK;
}

void Player::Release()
{
    // 플레이어 클래스에서는 이미지를 직접 해제하지 않음
    // (ImageManager에서 관리)
}

void Player::Update()
{
    if (!tilemap) return;

    // 입력 처리는 TakeTurn에서 처리됨
    
    // 이동 처리
    if (isMoving && !isTurnEnded)
    {
        moveTimer += TimerManager::GetInstance()->GetDeltaTime();
        
        // 이동 지연 시간이 지나면 다음 타일로 이동
        if (moveTimer >= moveDelay)
        {
            moveTimer = 0.0f;
            MoveToNextTile();
        }
    }

    // 플레이어 상태에 따른 프레임 업데이트
    switch (state)
    {
    case PlayerState::IDLE:
        frameX = 0;
        break;
    case PlayerState::MOVING:
        // 이동 방향에 따라 프레임 변경
        if (path.size() > currentPathIndex)
        {
            POINT nextPos = path[currentPathIndex];
            
            // 방향 결정 (상하좌우)
            if (nextPos.x > position.x) frameY = 1;      // 오른쪽
            else if (nextPos.x < position.x) frameY = 3; // 왼쪽
            else if (nextPos.y > position.y) frameY = 2; // 아래쪽
            else if (nextPos.y < position.y) frameY = 0; // 위쪽
            
            // 걷는 애니메이션
            frameX = (frameX + 1) % 4;
        }
        break;
    case PlayerState::ATTACKING:
        // 공격 애니메이션 (미구현)
        break;
    case PlayerState::USING_ITEM:
        // 아이템 사용 애니메이션 (미구현)
        break;
    }
}

void Player::Render(HDC hdc)
{
    if (!playerImage || !tilemap) return;

    // 플레이어 위치를 화면 좌표로 변환
    POINT screenPos = tilemap->WorldToScreen(position.x, position.y);
    
    // 타일 크기 계산 (카메라 줌 적용)
    int tileSize = (int)(TILE_SIZE * 1.0f); // 줌 레벨은 나중에 적용
    
    // 플레이어 렌더링
    playerImage->FrameRender(
        hdc, 
        screenPos.x, 
        screenPos.y, 
        frameX, 
        0
    );
}

void Player::TakeTurn()
{
    // 입력 처리
    HandleInput();
    
    // 행동에 따른 에너지 소모
    switch (state)
    {
    case PlayerState::MOVING:
        energy -= MOVE_COST;
        break;
    case PlayerState::ATTACKING:
        energy -= ATTACK_COST;
        break;
    case PlayerState::USING_ITEM:
        energy -= USE_ITEM_COST;
        break;
    default:
        // 아무 행동도 하지 않으면 최소 비용 차감
        energy -= MOVE_COST / 2;
        break;
    }
    
    // 에너지가 음수가 되지 않도록 보정
    if (energy < 0) energy = 0;
}

void Player::HandleInput()
{
    if (!tilemap) return;
    
    // 마우스 입력 처리
    if (KeyManager::GetInstance()->IsOnceKeyDown(VK_LBUTTON))
    {
        // 선택된 타일 가져오기
        POINT selectedTile = tilemap->GetSelectedTile();
        
        // 유효한 타일이 선택되었고, 이동 가능한 타일인지 확인
        if (selectedTile.x >= 0 && selectedTile.y >= 0 && 
            tilemap->IsWalkable(selectedTile.x, selectedTile.y))
        {
            // 목표 위치로 이동 명령
            MoveTo(selectedTile.x, selectedTile.y);
            
            // 상태 변경
            SetState(PlayerState::MOVING);
        }
    }
    
    // 공격 입력 처리 (예시)
    if (KeyManager::GetInstance()->IsOnceKeyDown('A'))
    {
        // 공격 처리 (미구현)
        SetState(PlayerState::ATTACKING);
    }
    
    // 아이템 사용 입력 처리 (예시)
    if (KeyManager::GetInstance()->IsOnceKeyDown('I'))
    {
        // 아이템 사용 처리 (미구현)
        SetState(PlayerState::USING_ITEM);
    }
}

void Player::SetTilemap(DungeonTilemap* tilemap)
{
    this->tilemap = tilemap;
    
    // 타일맵이 설정되면 플레이어 위치도 업데이트
    if (tilemap)
    {
        position.x = tilemap->GetMapWidth() / 2;
        position.y = tilemap->GetMapHeight() / 2;
        tilemap->SetPlayerPos(position.x, position.y);
    }
}

void Player::SetPosition(int x, int y)
{
    position.x = x;
    position.y = y;
    
    // 타일맵에도 플레이어 위치 업데이트
    if (tilemap)
    {
        tilemap->SetPlayerPos(position.x, position.y);
    }
}

void Player::MoveTo(int x, int y)
{
    if (!tilemap) return;
    
    // 이미 목표 위치에 있는 경우
    if (position.x == x && position.y == y)
    {
        return;
    }
    
    // 목표 위치 설정
    targetPosition.x = x;
    targetPosition.y = y;
    
    // 경로 찾기
    path = FindPath(position, targetPosition);
    
    // 경로가 존재하면 이동 시작
    if (!path.empty())
    {
        isMoving = true;
        currentPathIndex = 0;
        SetState(PlayerState::MOVING);
        isTurnEnded = false;
    }
}

// A* 알고리즘을 사용한 경로 찾기
std::vector<POINT> Player::FindPath(POINT start, POINT end)
{
    if (!tilemap) return std::vector<POINT>();
    
    // 맵 크기
    int mapWidth = tilemap->GetMapWidth();
    int mapHeight = tilemap->GetMapHeight();
    
    // 방향 배열 (상하좌우)
    const int dx[4] = { 0, 1, 0, -1 };
    const int dy[4] = { -1, 0, 1, 0 };
    
    // 방문 여부 배열
    bool** visited = new bool*[mapHeight];
    for (int i = 0; i < mapHeight; ++i)
    {
        visited[i] = new bool[mapWidth];
        memset(visited[i], 0, sizeof(bool) * mapWidth);
    }
    
    // 부모 노드 저장 배열
    POINT** parent = new POINT*[mapHeight];
    for (int i = 0; i < mapHeight; ++i)
    {
        parent[i] = new POINT[mapWidth];
        for (int j = 0; j < mapWidth; ++j)
        {
            parent[i][j].x = -1;
            parent[i][j].y = -1;
        }
    }
    
    // 우선순위 큐를 사용한 A* 알고리즘
    // pair: <f값, <x, y>> (f = g + h, g: 시작점에서의 거리, h: 목표점까지의 예상 거리)
    auto comparePairFirst = [](const std::pair<int, POINT>& a, const std::pair<int, POINT>& b) {
        return a.first > b.first;
    };

    std::priority_queue<std::pair<int, POINT>,
                        std::vector<std::pair<int, POINT>>,
                        decltype(comparePairFirst)> pq(comparePairFirst);
    
    // 시작점 추가
    pq.push(std::make_pair(0, start));
    visited[start.y][start.x] = true;
    
    bool foundPath = false;
    
    while (!pq.empty())
    {
        // 현재 노드
        int cost = pq.top().first;
        POINT current = pq.top().second;
        pq.pop();
        
        // 목표 도달
        if (current.x == end.x && current.y == end.y)
        {
            foundPath = true;
            break;
        }
        
        // 4방향 탐색
        for (int i = 0; i < 4; ++i)
        {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];
            
            // 맵 범위 체크
            if (nx < 0 || nx >= mapWidth || ny < 0 || ny >= mapHeight)
                continue;
            
            // 이미 방문했거나 이동할 수 없는 타일인 경우
            if (visited[ny][nx] || !tilemap->IsWalkable(nx, ny))
                continue;
            
            // 맨해튼 거리 (h값)
            int h = abs(nx - end.x) + abs(ny - end.y);
            // 시작점에서의 거리 (g값) = 현재까지의 비용 + 1
            int g = cost - abs(current.x - end.x) - abs(current.y - end.y) + 1;
            // f값 = g + h
            int f = g + h;
            
            // 다음 노드 추가
            POINT next = { nx, ny };
            pq.push(std::make_pair(f, next));
            visited[ny][nx] = true;
            parent[ny][nx] = current;
        }
    }
    
    // 경로 생성
    std::vector<POINT> path;
    
    if (foundPath)
    {
        // 목표에서 시작점까지 역추적
        POINT current = end;
        
        while (!(current.x == start.x && current.y == start.y))
        {
            path.push_back(current);
            current = parent[current.y][current.x];
        }
        
        // 경로 뒤집기 (시작점에서 목표까지)
        std::reverse(path.begin(), path.end());
    }
    
    // 메모리 해제
    for (int i = 0; i < mapHeight; ++i)
    {
        delete[] visited[i];
        delete[] parent[i];
    }
    delete[] visited;
    delete[] parent;
    
    return path;
}

void Player::MoveToNextTile()
{
    if (path.empty() || currentPathIndex >= path.size())
    {
        // 경로의 끝에 도달
        isMoving = false;
        SetState(PlayerState::IDLE);
        isTurnEnded = true;
        return;
    }
    
    // 다음 위치로 이동
    POINT nextPos = path[currentPathIndex];
    SetPosition(nextPos.x, nextPos.y);
    
    // 다음 경로 인덱스로 이동
    currentPathIndex++;
    
    // 경로의 끝에 도달했는지 확인
    if (currentPathIndex >= path.size())
    {
        isMoving = false;
        SetState(PlayerState::IDLE);
        isTurnEnded = true;
    }
    else
    {
        // 계속 이동 중
        SetState(PlayerState::MOVING);
    }
}

void Player::SetState(PlayerState newState)
{
    state = newState;
    
    // 상태에 따른 초기 프레임 설정
    switch (state)
    {
    case PlayerState::IDLE:
        frameX = 0;
        break;
    case PlayerState::MOVING:
        frameX = 0;
        break;
    case PlayerState::ATTACKING:
        frameX = 0;
        break;
    case PlayerState::USING_ITEM:
        frameX = 0;
        break;
    }
}
