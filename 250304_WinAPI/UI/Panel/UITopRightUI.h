#pragma once
#include "../Core/UIContainer.h"
#include "../BUtton/UIImageTextButton.h"
#include "../Text/UIText.h"
#include "../Util/UIResourceSubManager.h"

class UITopRightUI : public UIContainer {
private:
    UIImage* stairIcon = nullptr;
    UIText* stairText = nullptr;
    std::vector<UIImageTextButton*> menuButtons;

public:
    void Init(const D2D1_RECT_F& rect = { 922.f, 0.f, 1080.f, 62.32f }) {
        SetRect(rect);
        UIResourceSubManager::Preload_TopRightUI();

        AddStairInfo();
        AddMenuButtons();
    }

    void SetFloorText(const std::wstring& str) {
        if (stairText) stairText->SetText(str);
    }

    void SetMenuHandler(int index, std::function<void()> handler) {
        if (index >= 0 && index < menuButtons.size()) {
            // menuButtons[index]->SetOnClick(handler);
        }
    }

private:
    void AddStairInfo() {
        stairIcon = new UIImage();
        stairIcon->Init({ 0,0,29,31 });
        stairIcon->SetStyle(ImageStyle { D2DImageManager::GetInstance()->FindImage("stair_ico") });
        AddChild(stairIcon);

        stairText = new UIText();
        stairText->Init({ L"pixel", 16.0f, D2D1::ColorF(D2D1::ColorF::White) }, L"1층", { 0,31,29,62 });
        AddChild(stairText);
    }

    void AddMenuButtons() {
        const char* btns[] = { "guidebook_btn", "menu_btn" };
        const D2D_RECT_F coords[] = {
            { 29.f, 0, 95.68f, 62.32f },
            { 95.56f, 0, 158.0f, 62.32f }
        };

        const int slotCount = 2;

        for (int i = 0; i < slotCount; ++i) {
            auto* slot = new UIImageTextButton();
            slot->Init(coords[i]);
            menuButtons.push_back(slot);
            AddChild(slot);
            
            ImageStyle slotIconStyle =
            {
                D2DImageManager::GetInstance()->FindImage(btns[i])
            };
            
            auto* bgImage = new UIImage();
            bgImage->Init({0,0, slot->GetWidth(), slot->GetHeight()});
            bgImage->SetStyle(slotIconStyle);
            slot->AddChild(bgImage);
        }
        
    }
};
