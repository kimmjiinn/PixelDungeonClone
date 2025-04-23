#pragma once
#include <stack>
#include "../Text/UIText.h"

class UITextPool {
private:
    std::stack<UIText*> pool;

public:
    UIText* Get() {
        if (!pool.empty()) {
            auto* text = pool.top();
            pool.pop();
            return text;
        }
        return new UIText(); // 없으면 새로 생성
    }

    void Recycle(UIText* text) {
        if (!text) return;

        text->SetText(L"");
        pool.push(text);  // 다시 스택에 넣음
    }

    void Clear() {
        while (!pool.empty()) {
            delete pool.top();
            pool.pop();
        }
    }

    ~UITextPool() {
        Clear();
    }
};
