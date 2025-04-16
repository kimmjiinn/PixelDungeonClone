#pragma once
#include "GameObject.h"
#include "config.h"

enum class TileType
{
    FLOOR,      // 이동 가능한 바닥
    WALL,       // 이동 불가능한 벽
    DOOR,       // 문
    STAIRS_DOWN,// 아래층으로 내려가는 계단
    STAIRS_UP   // 위층으로 올라가는 계단
};

// 던전 타일 정보
typedef struct tagDungeonTile
{
    RECT rc;        // 타일 영역
    int frameX;     // 타일 이미지 프레임 X
    int frameY;     // 타일 이미지 프레임 Y
    TileType type;  // 타일 타입
    bool isVisible; // 플레이어에게 보이는지 여부
    bool isVisited; // 플레이어가 방문했는지 여부
} DUNGEON_TILE_INFO;

class Image;
class DungeonTilemap : public GameObject
{
private:
    Image* tileImage;                       // 타일 이미지
    DUNGEON_TILE_INFO* tiles;               // 타일 정보 배열
    int mapWidth;                           // 맵 가로 크기 (타일 수)
    int mapHeight;                          // 맵 세로 크기 (타일 수)
    
    POINT cameraPos;                        // 카메라 위치 (타일 단위)
    float cameraZoom;                       // 카메라 줌 레벨 (1.0f가 기본)
    
    POINT selectedTile;                     // 선택된 타일 위치
    POINT playerPos;                        // 플레이어 위치 (타일 단위)
    
    bool isInitialized;                     // 초기화 여부
    RECT rcMain;

public:
    virtual HRESULT Init() override;
    virtual void Release() override;
    virtual void Update() override;
    virtual void Render(HDC hdc) override;

    // 타일맵 초기화 (맵 크기 지정)
    HRESULT InitTilemap(int width, int height);
    
    // 타일 설정
    void SetTile(int x, int y, int frameX, int frameY, TileType type);
    
    // 타일 정보 가져오기
    DUNGEON_TILE_INFO GetTile(int x, int y);
    
    // 타일 타입 가져오기
    TileType GetTileType(int x, int y);
    
    // 타일 이동 가능 여부 확인
    bool IsWalkable(int x, int y);
    
    // 타일 가시성 설정
    void SetTileVisibility(int x, int y, bool isVisible);
    
    // 타일 방문 여부 설정
    void SetTileVisited(int x, int y, bool isVisited);
    
    // 카메라 위치 설정
    void SetCameraPos(int x, int y);
    
    // 카메라 이동
    void MoveCamera(int deltaX, int deltaY);
    
    // 카메라 줌 설정
    void SetCameraZoom(float zoom);
    
    // 카메라 줌 변경
    void ChangeZoom(float deltaZoom);
    
    // 월드 좌표를 화면 좌표로 변환
    POINT WorldToScreen(int worldX, int worldY);
    
    // 화면 좌표를 월드 좌표로 변환
    POINT ScreenToWorld(int screenX, int screenY);
    
    // 맵 저장
    void SaveMap(const wchar_t* fileName);
    
    // 맵 로드
    void LoadMap(const wchar_t* fileName);
    
    // 플레이어 위치 설정
    void SetPlayerPos(int x, int y);
    
    // 플레이어 위치 가져오기
    POINT GetPlayerPos() { return playerPos; }
    
    // 선택된 타일 위치 설정
    void SetSelectedTile(int x, int y);
    void UpdateVisibility();

    // 선택된 타일 위치 가져오기
    POINT GetSelectedTile() { return selectedTile; }
    
    // 맵 크기 가져오기
    int GetMapWidth() { return mapWidth; }
    int GetMapHeight() { return mapHeight; }

    DungeonTilemap();
    virtual ~DungeonTilemap();
};
