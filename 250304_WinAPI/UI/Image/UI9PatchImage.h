#pragma once
#include "UIBaseImage.h"
#include "../VisualStyle.h"


class UI9PatchImage : public UIBaseImage {
private:
    NinePatchStyle style;

public:
    void SetStyle(const NinePatchStyle& s) {
        style = s;
    }

    void Render(ID2D1HwndRenderTarget* rt) override {
        if (!isVisible || !isActive || !style.image || !rt) return;

        const auto dest = GetWorldRect();
        const float w = dest.right - dest.left;
        const float h = dest.bottom - dest.top;
        const float csX = style.cornerSize.width;
        const float csY = style.cornerSize.height;

        D2D1_RECT_F regionsDest[3][3] = {
            {{dest.left, dest.top, dest.left+csX, dest.top+csY},         {dest.left+csX, dest.top, dest.right-csX, dest.top+csY},       {dest.right-csX, dest.top, dest.right, dest.top+csY}},
            {{dest.left, dest.top+csY, dest.left+csX, dest.bottom-csY}, {dest.left+csX, dest.top+csY, dest.right-csX, dest.bottom-csY}, {dest.right-csX, dest.top+csY, dest.right, dest.bottom-csY}},
            {{dest.left, dest.bottom-csY, dest.left+csX, dest.bottom},  {dest.left+csX, dest.bottom-csY, dest.right-csX, dest.bottom},  {dest.right-csX, dest.bottom-csY, dest.right, dest.bottom}}
        };

        for (int y = 0; y < 3; ++y) {
            for (int x = 0; x < 3; ++x) {
                const int index = y * 3 + x;
                const auto& region = style.regions[index];
                const auto& dst = regionsDest[y][x];
                const FPOINT ws = GetWorldScale();

                style.image->RenderRaw(rt, dst, region.srcRect, ws.x, ws.y, 0.0f, false, false, region.alpha);
            }
        }

        // 🔸 출력 디버그용 외곽선
        ID2D1SolidColorBrush* debugBrush = nullptr;
        rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &debugBrush);
        rt->DrawRectangle(dest, debugBrush, 1.0f);
        if (debugBrush) debugBrush->Release();
    }
};
