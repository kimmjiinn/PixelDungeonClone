#pragma once
#include "../Core/UIComponent.h"

class IUIInteractable;
#include "../Util/IUIInteractable.h"

class UIContainerBase : public UIComponent {
protected:
    std::vector<UIComponent*> children;

public:
    UIContainerBase() = default;
    UIContainerBase(const UIContainerBase&) = delete;   // 댕글링 방지를 위한 것, 값 복사하면 안되요
    UIContainerBase& operator=(const UIContainerBase&) = delete; // 댕글링 방지를 위한 것, 값 복사하면 안되요
    
    ~UIContainerBase() override
    {
        ClearChild();
    }

    virtual void AddChild(UIComponent* child) {
        if (!child) return;
        child->SetParent(this);         // 자식에게 부모 지정
        children.push_back(child);
    }   

    void RemoveChild(UIComponent* target, bool isDelete = true) {
        if (!target) return;

        auto it = std::find(children.begin(), children.end(), target);
        if (it != children.end()) {
            children.erase(it);
            target->SetParent(nullptr); // 부모 해제
            if (isDelete)
            {
                delete target;
            }
            target = nullptr;
        }
    }
    void ClearChild() {
        for (auto* child : children) {
            if (!child) continue;
            child->SetParent(nullptr);
            delete child;
        }
        children.clear(); // 🔥 전부 비움
    }


    const std::vector<UIComponent*>& GetChildren() const {
        return children;
    }

    void SetRect(const D2D1_RECT_F& rect) override {
        UIComponent::SetRect(rect);  // ✅ 부모 위치 갱신

        // ✅ 자식 위치 재계산
        for (auto* c : children) {
            c->UpdateWorldRect();  // localRect → worldRect 변환
        }
    }

    void Update(float dt) override {
        if (!isActive) return;
        for (auto* c : children)
        {
            c->Update(dt);
        }
    }

    void Render(ID2D1HwndRenderTarget* rt) override {
        if (!isVisible || !isActive) return;
        
        for (auto* c : children)
        {
            c->Render(rt);
        }

        // TODO
        // DEBUG 끝나면 지우기
        D2D1_RECT_F rect = GetScaledDrawRect();
            
        // 🔸 출력 영역 확인용 사각형 (얇은 외곽선)
        ID2D1SolidColorBrush* debugBrush = nullptr;
        rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &debugBrush);
        rt->DrawRectangle(rect, debugBrush, 1.0f);  // 1.0f: 선 두께

        if (debugBrush) debugBrush->Release();
    }

    void UpdateWorldRect() override {
        UIComponent::UpdateWorldRect();
        for (auto* c : children)
            c->UpdateWorldRect();
    }

    bool HandleClick(int x, int y) override {
        if (!isActive) return false;

        // 🔄 역순 탐색
        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            auto* c = *it;
            if (!c || !c->IsActive()) continue;

            if (c->HandleClick(x, y)) return true;
        }
        return false;
    }

    // 클릭 자식 객체들 전부 가져오기
    std::vector<IUIInteractable*> GetInteractables() const {
        std::vector<IUIInteractable*> result;
        CollectInteractablesRecursive(this, result);
        return result;
    }
    // 자식 재귀 탐색용
    static void CollectInteractablesRecursive(const UIComponent* current, std::vector<IUIInteractable*>& result) {
        if (auto* interact = dynamic_cast<IUIInteractable*>(const_cast<UIComponent*>(current))) {
            result.push_back(interact);
        }

        if (auto* container = dynamic_cast<const UIContainerBase*>(current)) {
            for (auto* child : container->GetChildren()) {
                if (child) CollectInteractablesRecursive(child, result);
            }
        }
    }
};
