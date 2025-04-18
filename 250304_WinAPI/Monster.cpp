#include "Monster.h"
#include "Map.h"
#include "Game.h"

Monster::Monster()
{
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
}

void Monster::Render(HDC hdc)
{
}
