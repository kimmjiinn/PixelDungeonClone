#pragma once
// 태그용
class IUIInteractable {
public:
    virtual bool HandleClick(int x, int y) { return false; } 
    virtual ~IUIInteractable() = default;
};