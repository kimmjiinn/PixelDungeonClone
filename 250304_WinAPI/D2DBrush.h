#pragma once
#include <d2d1.h>
#include <wrl.h>

class D2DBrush {
private:
    Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> brush;

public:
    D2DBrush(D2D1_COLOR_F color, float alpha, ID2D1HwndRenderTarget* rt) {
        D2D1_COLOR_F finalColor = color;
        finalColor.a = alpha;
        rt->CreateSolidColorBrush(finalColor, &brush);
    }

    ID2D1Brush* Get() const { return brush.Get(); }
};
