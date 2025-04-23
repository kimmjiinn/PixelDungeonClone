#pragma once
#include <vector>
#include "../Core/UIComponent.h"

class UILayout {
public:
    virtual ~UILayout() = default;
    virtual void Apply(std::vector<UIComponent*>& children, const D2D1_RECT_F& parentRect) = 0;
};

/* 정렬 레이아웃 클래스들 */

class UIVerticalLayout : public UILayout {
private:
    float spacing = 0.0f;
    float padding = 0.0f;
    bool  bottomToTop = true;

public:
    UIVerticalLayout(float spacing = 0.0f, float padding = 0.0f)
        : spacing(spacing), padding(padding), bottomToTop(true) {
    }

    void Apply(std::vector<UIComponent*>& children, const D2D1_RECT_F& parentRect) override {

        if (bottomToTop) {
            float y = parentRect.bottom - padding;

            // ✅ 역순 순회: 최근 추가된 애가 아래로 가게
            for (auto it = children.rbegin(); it != children.rend(); ++it) {
                UIComponent* child = *it;
                D2D1_RECT_F r = child->GetLocalRect();
                float height = r.bottom - r.top;
                float width = r.right - r.left;

                y -= height;
                D2D1_RECT_F newRect = D2D1::RectF(r.left, y, r.left + width, y + height);
                child->SetRect(newRect);
                y -= spacing;
            }
        }
        else {
            float y = parentRect.top + padding;

            for (auto* child : children) {
                D2D1_RECT_F r = child->GetLocalRect();
                float height = r.bottom - r.top;
                float width = r.right - r.left;

                D2D1_RECT_F newRect = D2D1::RectF(r.left, y, r.left + width, y + height);
                child->SetRect(newRect);
                y += height + spacing;
            }
        }
    }
};

class UIHorizontalLayout : public UILayout {
private:
    float spacing = 0.0f;
    float paddingLeft = 0.0f;

public:
    UIHorizontalLayout(float spacing = 0.0f, float paddingLeft = 0.0f)
        : spacing(spacing), paddingLeft(paddingLeft) {
    }

    void Apply(std::vector<UIComponent*>& children, const D2D1_RECT_F& parentRect) override {
        float x = parentRect.left + paddingLeft;

        for (auto* child : children) {
            D2D1_RECT_F r = child->GetLocalRect();
            float width = r.right - r.left;
            float height = r.bottom - r.top;

            float y = parentRect.top; // 정렬 기준 위쪽

            D2D1_RECT_F newRect = D2D1::RectF(x, y, x + width, y + height);
            child->SetRect(newRect);

            x += width + spacing;
        }
    }
};

class UIGridLayout : public UILayout {
private:
    int columns = 1;
    float spacingX = 0.0f;
    float spacingY = 0.0f;
    float paddingLeft = 0.0f;
    float paddingTop = 0.0f;

public:
    UIGridLayout(int columns, float spacingX = 0.0f, float spacingY = 0.0f, float paddingLeft = 0.0f, float paddingTop = 0.0f)
        : columns(columns), spacingX(spacingX), spacingY(spacingY), paddingLeft(paddingLeft), paddingTop(paddingTop) {
    }

    void Apply(std::vector<UIComponent*>& children, const D2D1_RECT_F& parentRect) override {
        if (columns <= 0) return;

        float x = parentRect.left + paddingLeft;
        float y = parentRect.top + paddingTop;

        float startX = x;
        int col = 0;

        for (auto* child : children) {
            D2D1_RECT_F r = child->GetLocalRect();
            float width = r.right - r.left;
            float height = r.bottom - r.top;

            D2D1_RECT_F newRect = D2D1::RectF(x, y, x + width, y + height);
            child->SetRect(newRect);

            col++;
            if (col >= columns) {
                col = 0;
                x = startX;
                y += height + spacingY;
            }
            else {
                x += width + spacingX;
            }
        }
    }
};