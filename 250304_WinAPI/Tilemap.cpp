#include "TileMap.h"
#include "ImageManager.h"
#include "Image.h"
#include "AstarTile.h"

TileMap::TileMap()
    : tilesetImage(nullptr), startTile(nullptr), destTile(nullptr)
{
}

TileMap::~TileMap()
{
    Release();
}

HRESULT TileMap::Init()
{
    // 타일셋 이미지 로드
    tilesetImage = ImageManager::GetInstance()->AddImage(
        "tileset", L"Image/dungeon_tileset.bmp", 
        256, 256, 8, 8, true, RGB(255, 0, 255));
    
    // 타일맵 초기화
    for (int i = 0; i < TILE_COUNT_Y; i++)
    {
        for (int j = 0; j < TILE_COUNT_X; j++)
        {
            tiles[i][j].Init(j, i);
            
            // 타일 이미지 설정 (기본 바닥 타일)
            tiles[i][j].SetTileImage(tilesetImage);
            tiles[i][j].SetFrameX(rand() % 3); // 0~2 중 랜덤 바닥 타일
            tiles[i][j].SetFrameY(0);
        }
    }
    
    // 기본 시작 타일 설정
    SetStartTile(3, 3);
    
    return S_OK;
}

void TileMap::Release()
{
    // 특별히 해제할 자원 없음
    // tilesetImage는 ImageManager에서 관리됨
}

void TileMap::Update()
{
    // 타일 타입에 따른 이미지 프레임 업데이트
    for (int i = 0; i < TILE_COUNT_Y; i++)
    {
        for (int j = 0; j < TILE_COUNT_X; j++)
        {
            // 타일 타입에 따라 이미지 프레임 설정
            switch (tiles[i][j].GetType())
            {
            case AstarTile::TileType::None:
                // 기본 바닥 타일 유지 (0~2)
                break;
            case AstarTile::TileType::Start:
                tiles[i][j].SetFrameX(3); // 시작 타일
                break;
            case AstarTile::TileType::End:
                tiles[i][j].SetFrameX(5); // 목적지 타일
                break;
            case AstarTile::TileType::Wall:
                tiles[i][j].SetFrameX(6); // 벽 타일
                tiles[i][j].SetFrameY(1);
                break;
            }
            
            // 각 타일 업데이트
            tiles[i][j].Update();
        }
    }
}

void TileMap::Render(HDC hdc)
{
    // 모든 타일 렌더링
    for (int i = 0; i < TILE_COUNT_Y; i++)
    {
        for (int j = 0; j < TILE_COUNT_X; j++)
        {
            tiles[i][j].Render(hdc);
        }
    }
}

AstarTile* TileMap::GetTile(int x, int y)
{
    // 유효한 범위 체크
    if (x < 0 || x >= TILE_COUNT_X || y < 0 || y >= TILE_COUNT_Y)
        return nullptr;
        
    return &tiles[y][x];
}

AstarTile* TileMap::GetTileFromPos(int posX, int posY)
{
    // 위치를 타일 인덱스로 변환
    int tileX = posX / TILE_SIZE;
    int tileY = posY / TILE_SIZE;
    
    return GetTile(tileX, tileY);
}

void TileMap::SetTileType(int x, int y, AstarTile::TileType type)
{
    AstarTile* tile = GetTile(x, y);
    if (tile)
    {
        tile->SetType(type);
        
        // 타일 타입에 따라 이미지 프레임 설정
        switch (type)
        {
        case AstarTile::TileType::None:
            tile->SetFrameX(rand() % 3); // 0~2 중 랜덤 바닥 타일
            tile->SetFrameY(0);
            break;
        case AstarTile::TileType::Start:
            tile->SetFrameX(3); // 시작 타일
            tile->SetFrameY(0);
            break;
        case AstarTile::TileType::End:
            tile->SetFrameX(5); // 목적지 타일
            tile->SetFrameY(0);
            break;
        case AstarTile::TileType::Wall:
            tile->SetFrameX(6); // 벽 타일
            tile->SetFrameY(1);
            break;
        }
    }
}

void TileMap::LoadTileImages(const wchar_t* imagePath)
{
    // 타일셋 이미지 로드
    tilesetImage = ImageManager::GetInstance()->AddImage(
        "tileset", imagePath, 
        256, 256, 8, 8, true, RGB(255, 0, 255));
        
    // 모든 타일에 이미지 설정
    for (int i = 0; i < TILE_COUNT_Y; i++)
    {
        for (int j = 0; j < TILE_COUNT_X; j++)
        {
            tiles[i][j].SetTileImage(tilesetImage);
        }
    }
}

void TileMap::SetStartTile(int x, int y)
{
    // 기존 시작 타일이 있으면 일반 타일로 변경
    if (startTile)
    {
        startTile->SetType(AstarTile::TileType::None);
    }
    
    // 새 시작 타일 설정
    startTile = GetTile(x, y);
    if (startTile)
    {
        startTile->SetType(AstarTile::TileType::Start);
        startTile->SetFrameX(3); // 시작 타일 이미지
        startTile->SetFrameY(0);
    }
}

void TileMap::SetDestTile(int x, int y)
{
    // 기존 목적지 타일이 있으면 일반 타일로 변경
    if (destTile)
    {
        destTile->SetType(AstarTile::TileType::None);
    }
    
    // 새 목적지 타일 설정
    destTile = GetTile(x, y);
    if (destTile)
    {
        destTile->SetType(AstarTile::TileType::End);
        destTile->SetFrameX(5); // 목적지 타일 이미지
        destTile->SetFrameY(0);
    }
}
