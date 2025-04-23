#pragma once
#include "../Core/UIContainer.h"
#include "../Text/UITextBox.h"
#include "../Button/UIImageTextButton.h"
#include "../Core/UILayout.h"
#include "../VisualStyle.h"
#include "../Util/UIResourceSubManager.h"
#include "../Test/UITestEffectManager.h"
#include "../Util/UIHelper.h"
#include "../Image/UI9PatchImage.h"

class UIInventory : public UIContainer {
private:
    UITextBox* titleText = nullptr;
    UIContainer* gridArea = nullptr;
    UIContainer* bottomSection = nullptr;
    
    const int numCols = 5;
    const int numRows = 5;
    const D2D1_SIZE_F slotSize = {60.f,60.f};
    const float gridSpacing = 10.0f;
    const D2D1_RECT_F padding = {20,0,21, 0 };

public:
    void Init(const D2D1_RECT_F& area = { 200, 100, 578, 569 }) {
        SetRect(area);
        UIResourceSubManager::Preload_InventoryUI();

        AddBackGround();
        AddTitleSection();
        AddGridSection();
        AddBottomSection();
    }

private:
    void AddBackGround() {
        UIResourceSubManager::Preload_NinePatch();
        NinePatchStyle defaultNinePatchStyle =
            UIHelper::CreateNinePatchFromSheet("chrome", "chrome", {6.0f, 6.0f});
        
        auto* inventoryBox = UIHelper::ApplyNinePathStyle(this, this->GetSizeRect(), defaultNinePatchStyle);
    }
    
    void AddTitleSection() {
        UIButtonStyle titleStyle = {
            {},  // 배경 없음
            { L"pixel", 18.0f, D2D1::ColorF::White, true,
              DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER }
        };

        titleText = new UITextBox();
        titleText->Init(titleStyle, L"배낭", { padding.left, 5, GetWidth(), 50 });
        AddChild(titleText);

        float imgSize = 30;
        D2D1_RECT_F dummyRect = { GetWidth() - padding.right - imgSize, 5, GetWidth() - padding.right, 5 + imgSize};
        UIIconStyle defaultIconStyle =
        {
        { D2DImageManager::GetInstance()->FindImage("exit_btn")},
        { D2DImageManager::GetInstance()->FindImage("")}
        };

        auto* exitBtn = UIHelper::ApplyIconStyle(this, dummyRect, defaultIconStyle);
        exitBtn->SetOnClick([this, exitBtn]() {
            wstring debugString = (L"클릭 [닫기] 클릭\n");
            UITestEffectManager::GetInstance()->AddEffect(debugString, exitBtn->GetWorldRect());
            this->SetActive(false);
            });
    }

    void AddGridSection() {
        gridArea = new UIContainer();
        gridArea->SetRect({ 0, 50, GetWidth(), 50 + (slotSize.width + gridSpacing) * numRows } );
        gridArea->SetLayout(new UIGridLayout(numCols, gridSpacing, gridSpacing, padding.left, padding.top));
        AddChild(gridArea);

        for (int i = 0; i < numCols * numRows; ++i) {
            D2D1_RECT_F dummyRect = { 0, 0, slotSize.width, slotSize.height };
            UIInventorySlotStyle inventorySlotStyle =
                {
                    { D2DImageManager::GetInstance()->FindImage("inventory_slot"), },
                    { nullptr },
                    { L"pixel", 14.0f, D2D1::ColorF::White },
                    { L"pixel", 14.0f, D2D1::ColorF::White }
                };  // 예시 스타일
            auto* slot = UIHelper::ApplyInventorySlotStyle(gridArea, dummyRect, inventorySlotStyle);

            slot->SetOnClick([this, i, slot]()
            {
                wstring debugString = (L"클릭 대상 [" + to_wstring(i) + L"] 클릭\n");
                UITestEffectManager::GetInstance()->AddEffect(debugString, slot->GetWorldRect());
            });
        }

        gridArea->UpdateLayout();
    }

    void AddBottomSection() {
        const int col = 2;
        const float padding = 10.0f;
        const float width = (GetWidth() - padding * (col + 1)) / col;

        UIIconStyle defaultIconStyle =
            {
            { D2DImageManager::GetInstance()->FindImage("default_btn")},
            { D2DImageManager::GetInstance()->FindImage(""), {62,0,width-62,0 }}
            };
        
        bottomSection = new UIContainer();
        bottomSection->SetRect({0, GetHeight() - 60, GetWidth(), GetHeight() });
        bottomSection->SetLayout(new UIHorizontalLayout(9, 20));
        AddChild(bottomSection);


        D2D1_RECT_F dummyRect = { 0, 0, 164, 40 };
        
        auto* btn1 = UIHelper::ApplyIconStyle(bottomSection, dummyRect, defaultIconStyle);
        btn1->SetOnClick([this, btn1]() {
            wstring debugString = (L"클릭 [BTN1] 클릭\n");
            UITestEffectManager::GetInstance()->AddEffect(debugString, btn1->GetWorldRect());
            });

        auto* btn2 = UIHelper::ApplyIconStyle(bottomSection, dummyRect, defaultIconStyle);
        btn2->SetOnClick([this, btn2]() {
            wstring debugString = (L"클릭 [BTN2] 클릭\n");
            UITestEffectManager::GetInstance()->AddEffect(debugString, btn2->GetWorldRect());
            });


        bottomSection->UpdateLayout();
    }
};
