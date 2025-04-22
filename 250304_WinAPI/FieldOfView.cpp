#include "FieldOfView.h"
#include "AstarScene.h"
#include <cmath>

void FieldOfView::Calculate(AstarTile(&map)[20][20], int tileIdX, int tileIdY, int checkRow, float startSlope, float endSlope, ScanDirection direction)
{
	// 한 분면 검사
	/*
	기울기
	(y1-y2) / (x1-x2)
	y값 증가량에 대한 x값을 알아야 하기 때문에 역수로 계산한다.(x = 1/m * y)
	*/

	if (endSlope > startSlope)
		return;
	
	float nextStartSlope = startSlope;
	bool isBlock = false;

	// 탐색용 기울기
	float exploreStartSlope;
	float exploreEndSlope;

	// dy 맵 크기를 벗어나면 나올 수 없는 범위의 좌표가 나옴->왼쪽부터 차례대로...
	for (int dy = checkRow; dy < ASTAR_TILE_COUNT; dy++) // 탐색 단위
	{
		for (int dx = ceil((float)dy * nextStartSlope); dx >= 0; dx--) // 시작 기울기->끝기울기까지
		{
			// 타일 좌표
			int idX = tileIdX + (-dx * direction.horizonX) + (dy * direction.horizonY);
			int idY = tileIdY + (-dx * direction.verticalX) + (dy * direction.verticalY);

			if (idX < 0 || idX >= ASTAR_TILE_COUNT || idY < 0 || idY >= ASTAR_TILE_COUNT)
				continue;
	
			// 타일이 시야 내에 있는지 확인
			float centerSlope = (float)dx / (float)dy;
			// 접점 기울기-dy에 -+0.5 left, right 바꾸면 다르게 나옴 
			float leftSlope = ((float)dx + 0.5f) / ((float)dy + 0.5f);
			float rightSlope = ((float)dx - 0.5f) / ((float)dy - 0.5f);

			if (rightSlope > startSlope)
			{
				continue;
			}
			
			if (leftSlope <= endSlope)
			{
				break;
			}

			if (centerSlope > startSlope)
			{
				continue;
			}

			if (centerSlope < endSlope)
			{
				break;
			}

			// 타일의 leftSlope은 dy+1 타일의 rightSlope과 같다.
			if (endSlope == rightSlope)
			{
				//endSlope = leftSlope;
				endSlope = centerSlope;
				/*break;*/
			}

			AstarTile* tile ;
			tile = &map[idY][idX];

			if (!tile)
				continue;
	
			tile->isVisible = true;

			// 타일이 벽이라면
			if (isBlock)
			{
				if (tile->GetType() == AstarTileType::Wall)
				{
					if (dx == 0)
					{
						Calculate(map, tileIdX, tileIdY, dy + 1, exploreStartSlope, exploreEndSlope, direction);
						return;
					}
					nextStartSlope = rightSlope;
					continue;
				}
				else
				{
					Calculate(map, tileIdX, tileIdY, dy + 1, exploreStartSlope, exploreEndSlope, direction);
					//Calculate(map, tileIdX, tileIdY, dy + 1, nextStartSlope, leftSlope, direction);
					// dx == 0이면 rightSlope이 마이너스. start가 end보다 작기 때문에 종료된다.
					if (dx == 0)
					{
						nextStartSlope = 0.0f;
					}
					else
					{
						nextStartSlope = rightSlope;
					}
					startSlope = nextStartSlope;
					isBlock = false;
				}
			}
			else
			{
				if (tile->GetType() == AstarTileType::Wall)
				{	
					exploreStartSlope = nextStartSlope;
					exploreEndSlope = leftSlope;
					Calculate(map, tileIdX, tileIdY, dy + 1, nextStartSlope, leftSlope, direction);

					if (dx == 0)
					{
						return;
					}
					isBlock = true;
					continue;
				}
			}
			tile->SetColor(RGB(0, 255, 0));
		}
	}
}
