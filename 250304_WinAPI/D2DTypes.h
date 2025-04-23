#pragma once
#include <d2d1.h>
#include <vector>

class D2DImage;

/* 시트 고정 이미지 분할 용 */
struct D2DImageFrame {
    D2DImage* sourceImage = nullptr;
    D2D1_RECT_F sourceRect{};  // 해당 키에 해당하는 잘라낼 영역
};

/* 시트 애니메이션 이미지 분할 용 */
struct D2DAnimationFrame {
    D2D1_RECT_F sourceRect;  // 이미지 내의 해당 프레임 위치
    float duration = 0.1f;   // 각 프레임의 유지 시간 (초)
};
using D2DAnimationClip = std::vector<D2DAnimationFrame>;