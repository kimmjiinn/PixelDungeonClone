#include "Monster.h"
#include "Map.h"
#include "Game.h"
#include "Image.h"

Monster::Monster()
{
    image = ImageManager::GetInstance()->AddImage("rat_Idle", L"Image/rat_Idle.bmp", 16 * 4, 16, 4, 1, true, RGB(255, 0, 255));
    maxFrame = 4;
    currFrame = 0;
    pos = { 15, 15 };
}

void Monster::Move(int dx, int dy, Map map)
{
}

void Monster::Act(Game game)
{
    //EndTurn();
}

bool Monster::NeedInput()
{
    return false;
}

// 렌더용
void Monster::Update()
{
    currFrame++;
    if (currFrame >= maxFrame)
    {
        currFrame = 0;
    }
}

void Monster::Render(HDC hdc)
{
    image->FrameRender(hdc, pos.x, pos.y, currFrame, 0);
}
