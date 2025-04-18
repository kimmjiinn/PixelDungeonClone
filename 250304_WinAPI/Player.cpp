#include "Player.h"

void Player::Move(int dx, int dy)
{
	// 키보드 입력 받은 값을 설정
	pos.x = dx;
	pos.y = dy;
}
