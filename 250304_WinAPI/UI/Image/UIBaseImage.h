#pragma once
#include "../Core/UIComponent.h"

// 공통 이미지 렌더 컴포넌트 기반 클래스
class UIBaseImage : public UIComponent {
protected:
    float alpha = 1.0f;
    D2D1_COLOR_F tint = D2D1::ColorF(1, 1, 1, 1);

public:
    void SetAlpha(float a) { alpha = a; }
    void SetTint(D2D1_COLOR_F color) { tint = color; }

    float GetAlpha() const { return alpha; }
    D2D1_COLOR_F GetTint() const { return tint; }

    virtual void Render(ID2D1HwndRenderTarget* rt) override = 0;
};