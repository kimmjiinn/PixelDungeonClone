﻿#pragma once
#include "../Core/UIContainer.h"
#include "../Text/UIText.h"
#include "../Util/UITextPool.h"
#include <deque>
#include <string>

class UITextLogPanel : public UIContainer {
private:
    int maxLines = 10;
    D2D_RECT_F lineRect;
    UITextPool pool;

public:
    ~UITextLogPanel() override
    {
        Clear();
    }

    void Init(const D2D1_RECT_F& area, int maxLine = 5, float lineHeight = 10.0f, float spacing = 2.0f) {
        SetRect(area);
        SetMaxLineAndHeight(maxLine, lineHeight);
        // 세로 정렬, 아래에서 위로 쌓이도록 역순 삽입
        SetLayout(new UIVerticalLayout(spacing, spacing));
    }
    void SetMaxLineAndHeight(int maxLine, float lineHeight)
    {
        maxLines = maxLine;
        lineRect = { 0, 0, GetWidth(), lineHeight};
    }

    void AddLog(const std::wstring& content, const TextStyle& style) {
        // 🔁 기존 텍스트 재활용
        UIText* text = pool.Get();
        text->SetText(content);
        text->SetStyle(style);
        text->SetLocalRect(lineRect);

        // UIContainer가 자식 자동 정렬
        AddChild(text);

        // 최대 줄 수 초과 시 제거 + 재활용
        if (GetChildren().size() > maxLines) {
            auto* oldest = GetChildren().front(); // 앞쪽이 가장 오래된 줄
            oldest->SetRect({ 0,0,0,0 });
            RemoveChild(oldest, false); // 자식의 메모리 해제는 x
            pool.Recycle(dynamic_cast<UIText*>(oldest));
        }
        UpdateLayout();

    }

    void Clear() {
        for (auto* c : GetChildren()) {
            RemoveChild(c);
            pool.Recycle(dynamic_cast<UIText*>(c));
        }
    }
};
