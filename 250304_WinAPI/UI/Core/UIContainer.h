#pragma once
#include "../Core/UIContainerBase.h"
#include "../Core/UILayout.h"

class UIContainer : public UIContainerBase {
protected:
    UILayout* layout = nullptr;

public:
    void AddChild(UIComponent* child) override
    {
        UIContainerBase::AddChild(child);
    }

    void SetLayout(UILayout* l) {
        layout = l;
        UpdateLayout();
    }

    void UpdateLayout() {
        if (layout)
            layout->Apply(children, GetSizeRect());

        for (auto* c : children)
            c->UpdateWorldRect();
    }

    void SetRect(const D2D1_RECT_F& rect) override {
        UIComponent::SetRect(rect);  // ✅ 부모 위치 갱신

        UpdateLayout();
    }
    
};

class UIAutoContainer : public UIContainer {
public:
    UIAutoContainer()
    {
        layout = new UIVerticalLayout();
    }

    ~UIAutoContainer() override
    {
        layout = nullptr;
    }
    
    void AddChild(UIComponent* child) override
    {
        UIContainerBase::AddChild(child);

        // ✅ 자동 확장용
        float totalHeight = 0.0f;
        for (auto* c : children)
        {
            D2D1_RECT_F r = c->GetLocalRect();
            totalHeight += (r.bottom - r.top); // 혹은 고정 lineHeight + spacing
        }

        D2D1_RECT_F local = GetLocalRect();
        SetRect(D2D1::RectF(local.left, local.top, local.right, local.top + totalHeight));
        UpdateLayout();
    }
};
