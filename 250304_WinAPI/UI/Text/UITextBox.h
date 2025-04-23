#pragma once
#include "../Core/UIContainerBase.h"
#include "../VisualStyle.h"
#include "../Image/UIImage.h"
#include "../Text/UIText.h"

// ✅ 복합 UI: 배경 + 텍스트
class UITextBox : public UIContainerBase {
private:
    UIButtonStyle style;
    UIText* text = nullptr;
    UIImage* bgImage = nullptr;

public:
    void Init(const UIButtonStyle& s, const std::wstring& content, const D2D1_RECT_F& layout) {
        style = s;
        SetRect(layout); // 자신의 위치 설정

        float width = layout.right - layout.left;
        float height = layout.bottom - layout.top;

        if (style.background.image) {
            bgImage = new UIImage();
            bgImage->Init({ 0, 0, width, height });
            bgImage->SetStyle(style.background);
            AddChild(bgImage); // 배경도 자식으로
        }

        text = new UIText();
        text->Init(style.textStyle, content, { 0, 0, width, height });
        AddChild(text);
    }
 
    void SetText(const std::wstring& t) {
        if (text) text->SetText(t);
    }

    void SetRect(const D2D1_RECT_F& r) {
        UIComponent::SetRect(r);
        if (text) text->Update(1.0f);

    }

    void Update(float dt) override {

    }

    void Render(ID2D1HwndRenderTarget* rt) override {

        D2D1_RECT_F rect = GetScaledDrawRect();

        if (style.background.image)
        {
            D2D1_RECT_F rect = GetScaledDrawRect();
            auto ws = GetWorldScale();
            style.background.image->RenderFrameScale(rect.left, rect.top, ws.x, ws.y, 0, 0, 0.0f, false, false, style.background.alpha);
        }

        if (text)
        {
            text->Render(rt);
        }
    }
};
