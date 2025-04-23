#pragma once
#include "../../config.h"

class UIComponent {
protected:
    UIComponent* parent = nullptr;

    D2D1_RECT_F localRect{};   // 부모 기준 위치
    D2D1_RECT_F worldRect{};   // 실제 화면 위치
    FPOINT scale = { 1.0f , 1.0f };

    bool isVisible = true;  // 렌더링 여부
    bool isActive = true;   // Update 여부

public:
    virtual ~UIComponent() = default;

    virtual void Update(float dt) {
        if (!isActive) return;
        // 기본 Update 내용 또는 순수 가상으로 유지
    }

    virtual void Render(ID2D1HwndRenderTarget* rt) {
        if (!isVisible || !isActive) return;
        // 기본 Render 내용 또는 순수 가상으로 유지
    }

    virtual void Init(const D2D1_RECT_F& rect)
    {
        SetRect(rect);
    }
    
    virtual bool HandleClick(int x, int y) { return false; }

    virtual void SetScale(FPOINT s) {
        this->scale = s;
    }
    
    FPOINT GetScale() const { return scale; }

    // ✅ scale 반영된 실제 그릴 영역 반환
    D2D1_RECT_F GetScaledDrawRect() const {
        D2D1_RECT_F r = GetWorldRect();
        FPOINT ws = GetWorldScale();

        float width = (r.right - r.left) * ws.x;
        float height = (r.bottom - r.top) * ws.y;

        float centerX = (r.left + r.right) * 0.5f;
        float centerY = (r.top + r.bottom) * 0.5f;

        return D2D1::RectF(
            centerX - width * 0.5f,
            centerY - height * 0.5f,
            centerX + width * 0.5f,
            centerY + height * 0.5f
        );
    }


    // 월드 scale 반영 (재귀)
    FPOINT GetWorldScale() const {
        if (parent)
        {
            FPOINT ps = parent->GetWorldScale();
            return { scale.x * ps.x, scale.y * ps.y };
        }
        return scale;
    }


    // 절대 좌표 지정
    virtual void SetWorldRect(const D2D1_RECT_F& rect) {
        worldRect = rect;
        UpdateLocalRect();
    }

    void SetLocalRect(const D2D1_RECT_F& rect) { SetRect(rect); }
    virtual void SetRect(const D2D1_RECT_F& rect) {
        localRect = rect;
        UpdateWorldRect();
    }

    void SetParent(UIComponent* p) {
        parent = p;
        UpdateWorldRect();

    }

    virtual void UpdateLocalRect() {
        float width = worldRect.right - worldRect.left;
        float height = worldRect.bottom - worldRect.top;

        if (parent) {
            D2D1_RECT_F parentWorld = parent->GetWorldRect();

            localRect.left = worldRect.left - parentWorld.left;
            localRect.top = worldRect.top - parentWorld.top;
            localRect.right = localRect.left + width;
            localRect.bottom = localRect.top + height;
        }
        else {
            localRect = D2D1::RectF(0, 0, width, height);
        }
    }

    virtual void UpdateWorldRect() {
        if (parent) {
            D2D1_RECT_F parentWorld = parent->GetWorldRect();

            float width = localRect.right - localRect.left;
            float height = localRect.bottom - localRect.top;

            worldRect.left = parentWorld.left + localRect.left;
            worldRect.top = parentWorld.top + localRect.top;
            worldRect.right = worldRect.left + width;
            worldRect.bottom = worldRect.top + height;
        }
        else {
            worldRect = localRect;
        }
    }

    float GetWidth() const
    {
        return localRect.right - localRect.left;
    }
    float GetHeight() const
    {
        return localRect.bottom - localRect.top;
    }
    D2D1_RECT_F GetWorldRect() const { return worldRect; }
    D2D1_RECT_F GetLocalRect() const { return localRect; }
    D2D1_RECT_F GetSizeRect() const { return { 0, 0, GetWidth(), GetHeight() }; }

    void SetVisible(bool visible) { isVisible = visible; }
    bool IsVisible() const { return isVisible; }

    void SetActive(bool active) { isActive = active; }
    bool IsActive() const { return isActive; }
};
