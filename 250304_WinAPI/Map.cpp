#include "Map.h"
#include "CommonFunction.h"
#include <functional>

#include "algorithm"


HRESULT Tile::Init()
{
	return E_NOTIMPL;
}

HRESULT Tile::Init(int idX, int idY)
{
	this->idX = idX;
	this->idY = idY;

	center.x = idX * TILE_SIZE + (TILE_SIZE / 2);
	center.y = idY * TILE_SIZE + (TILE_SIZE / 2);

	rc.left = idX * TILE_SIZE;
	rc.right = rc.left + TILE_SIZE;
	rc.top = idY * TILE_SIZE;
	rc.bottom = rc.top + TILE_SIZE;

	type = TileType::None;

	color = RGB(100, 100, 100);
	hBrush = CreateSolidBrush(color);

	return S_OK;
}

void Tile::Release()
{
	DeleteObject(hBrush);
}

void Tile::Update()
{
}

void Tile::Render(HDC hdc)
{
	hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	FillRect(hdc, &rc, hBrush);
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

	SelectObject(hdc, hOldBrush);
}

void Tile::SetColor(COLORREF color)
{
	this->color = color;
	DeleteObject(hBrush);

	hBrush = CreateSolidBrush(color);
}

HRESULT Map::Init()
{
	for (int i = 0; i < TILE_COUNT; i++)
	{
		for (int j = 0; j < TILE_COUNT; j++)
		{
			tiles[i][j].Init(j, i);
		}
	}
	return S_OK;
}

void Map::Release()
{
}

void Map::Update()
{
	if (KeyManager::GetInstance()->IsStayKeyDown(VK_RBUTTON))
	{
		// g_ptMouse로 인덱스를 계산
		int x, y;
		x = g_ptMouse.x / TILE_SIZE;
		y = g_ptMouse.y / TILE_SIZE;

		if (0 <= x && x < TILE_COUNT &&
			0 <= y && y < TILE_COUNT)
		{
			if (tiles[y][x].GetType() != Tile::TileType::Start &&
				tiles[y][x].GetType() != Tile::TileType::End)
			{
				tiles[y][x].SetColor(RGB(100, 150, 100));
				tiles[y][x].SetType(Tile::TileType::Wall);
			}
		}
	}
}

void Map::Render(HDC hdc)
{
	for (int i = 0; i < TILE_COUNT; i++)
	{
		for (int j = 0; j < TILE_COUNT; j++)
		{
			tiles[i][j].Render(hdc);
		}
	}
}