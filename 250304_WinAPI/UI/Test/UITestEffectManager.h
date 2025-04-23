#pragma once
#include "../Text/UITextEffect.h"
#include "../../Singleton.h"
#include <vector>

class UITestEffectManager : public Singleton<UITestEffectManager>  {
private:
    std::vector<UITextEffect*> effects;

    TextStyle defaultTextStyle = {
            L"pixel", 16.0f, D2D1::ColorF(D2D1::ColorF::White), true,
             DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER
    };

    EffectStyle defaultEffectStyle =
    {
        20.0f, // duration
        -50.0f // y movespeed value
    };

public:
    ~UITestEffectManager() {
        for (auto* fx : effects) {
            delete fx;
        }
        effects.clear();
    }

    void AddEffect(const std::wstring& text, const D2D1_RECT_F& rect)
    {
        auto* fx = new UITextEffect();
        fx->SetWorldRect(rect);
        fx->Init(text, defaultTextStyle, defaultEffectStyle);
        effects.push_back(fx);
    }

    void AddEffect(const std::wstring& text, const TextStyle& style, const D2D1_RECT_F& rect, const EffectStyle& effectStyle) {
        auto* fx = new UITextEffect();
        fx->SetWorldRect(rect);
        fx->Init(text, style, effectStyle);
        effects.push_back(fx);
    }

    void Update(float dt) {
        for (auto it = effects.begin(); it != effects.end(); ) {
            if ((*it)->IsDead()) {
                delete *it;
                it = effects.erase(it);
            }
            else {
                (*it)->Update(dt);
                ++it;
            }
        }
    }

    void Render(ID2D1HwndRenderTarget* rt) {
        for (auto* fx : effects) {
            if (fx)
                fx->Render(rt);
        }
    }

    void Clear() {
        for (auto* fx : effects) {
            delete fx;
        }
        effects.clear();
    }
};
