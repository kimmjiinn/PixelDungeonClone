#include "FieldOfView.h"
#include <cmath>
#include "CommonFunction.h"

void FieldOfView::Calculate(Map(&map)[TILE_Y][TILE_X], int viewRange, int tileIdX, int tileIdY, int checkRow, float startSlope, float endSlope, ScanDirection direction)
{
	// 한 분면 검사
	/*
	기울기
	(y1-y2) / (x1-x2)
	y값 증가량에 대한 x값을 알아야 하기 때문에 역수로 계산한다.(x = 1/m * y)
	*/

	if (viewRange <= 0)
	return;

	RECT viewRc;
	RECT tileRc;

	if (endSlope > startSlope) return;

	for (int depth = checkRow; depth < TILE_Y; ++depth) {
		float nextStart = startSlope;
		bool blocked = false;
		int dy = -depth;
		int exploreStartSlope = startSlope;

		for (int dx = -depth*startSlope; dx <= 0; ++dx) {
			// 1) 슬로프 계산
			float leftSlope = (dx - 0.5f) / (dy + 0.5f);
			float rightSlope = (dx + 0.5f) / (dy - 0.5f);
			if (rightSlope > startSlope) continue;
			if (leftSlope < endSlope)      break;

			// 2) 월드 좌표 변환
			int idX = tileIdX + dx * direction.horizonX + -dy * direction.horizonY;
			int idY = tileIdY + dx * direction.verticalX + -dy * direction.verticalY;
			if (idX < 0 || idX >= TILE_X || idY < 0 || idY >= TILE_Y)
				continue;

			Map& tile = map[idY][idX];

			//viewRc = GetRectAtCenter(tileIdX, tileIdY, viewRange * TILE_SIZE, viewRange * TILE_SIZE);
			//tileRc = GetRectAtCenter(idX, idY, TILE_SIZE, TILE_SIZE);

			//if (!RectInRect(viewRc, tileRc))
			//{
			//	break;
			//}

			// 3) 비차단 타일 표시
			if (!blocked && tile.type != 0) {
				tile.visible = true;
				//tile.SetColor(RGB(0, 255, 0));
			}

			// 4) 벽 발견 시 재귀 분기
			if (tile.type == 0) {
				blocked = true;
				Calculate(map, viewRange, tileIdX, tileIdY,
					depth + 1, exploreStartSlope, leftSlope, direction);
				nextStart = rightSlope;
				exploreStartSlope = (leftSlope >= 1.0f ? 0.99999f : leftSlope);
			}
			// 5) 그림자 끝 지점 복귀
			else if (blocked && tile.type == 0) {
				blocked = false;
				startSlope = nextStart;
				tile.visible = true;
				//tile.SetColor(RGB(0, 255, 0));
			}
		}

		// 6) 행 전체가 차단되었으면 종료
		if (blocked) break;
	}
}