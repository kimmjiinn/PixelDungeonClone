#include "Player.h"
#include "Map.h"
#include "Image.h"

Player::Player()
{
	graphicID = 1;
	image = ImageManager::GetInstance()->AddImage(
		"dalia", L"Image\\Dalia(Child).Textures.bmp", 2736, 48, 57, 1, true, RGB(255, 0, 255));

	maxFrame = 57;
	currFrame = 0;
	pos = { 15, 15 };
}

void Player::Move(int dx, int dy, Map map)
{
	// 키보드 입력 받은 값을 설정
	int newX = pos.x + dx;
	int newY = pos.y + dy;

	if (newX <= 0 || newX >= map.GetWidth() || newY <= 0 || newY >= map.GetHeight())
		return;
	if (map.GetTileType(newX/30, newY/30) != Tile::TileType::Wall)
	{
		pos.x = newX;
		pos.y = newY;
	}
}

// 렌더에 필요한 것
void Player::Update()
{
	currFrame++;
	if (currFrame >= maxFrame)
	{
		currFrame = 0;
	}
}

void Player::Render(HDC hdc)
{
	image->FrameRender(hdc, pos.x, pos.y, currFrame, 0);
}
