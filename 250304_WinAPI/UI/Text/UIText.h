#pragma once
#include "../Core/UIComponent.h"
#include "../VisualStyle.h"
#include "../../DWriteFactory.h"

class UIText : public UIComponent {
private:
    std::wstring text;
    TextStyle style;

    IDWriteTextFormat* format = nullptr;
    ID2D1SolidColorBrush* brush = nullptr;

public:
    ~UIText() override {
        if (format) format->Release();
        if (brush) brush->Release();
    }

    void Init(const TextStyle& s, const std::wstring& txt, const D2D1_RECT_F& layout) {
        SetRect(layout);

        text = txt;
        style = s;

        DWriteFactory::GetInstance()->CreateTextFormat(
            style.fontName,
            &format,
            style.fontSize,
            style.bold,
            style.horizontalAlign,
            style.verticalAlign
        );
    }

    void SetText(const std::wstring& txt) {
        text = txt;
    }

    void SetStyle(const TextStyle& s) {
        style = s;
        if (format) { format->Release(); format = nullptr; }

        DWriteFactory::GetInstance()->CreateTextFormat(
            style.fontName,
            &format,
            style.fontSize,
            style.bold,
            style.horizontalAlign,
            style.verticalAlign
        );

        if (brush) { brush->Release(); brush = nullptr; }
    }

    TextStyle& GetStyle() {
        return style;
    }

    void Render(ID2D1HwndRenderTarget* rt) override {
        if (!isVisible || !isActive) return;
        if (text.length() < 1) return;
        
        if (!brush && rt)
        {
            rt->CreateSolidColorBrush(style.color, &brush);
        }
        if (format && brush)
        {
            D2D1_RECT_F rect = GetScaledDrawRect();
            rt->DrawTextW(text.c_str(), static_cast<UINT32>(text.length()), format, &rect, brush);
            
            // 🔸 출력 영역 확인용 사각형 (얇은 외곽선)
            ID2D1SolidColorBrush* debugBrush = nullptr;
            rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &debugBrush);
            rt->DrawRectangle(rect, debugBrush, 1.0f);  // 1.0f: 선 두께

            if (debugBrush) debugBrush->Release();
            
        }


    }

};
