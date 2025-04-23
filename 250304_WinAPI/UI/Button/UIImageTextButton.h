#pragma once

#include "../Core/UIContainerBase.h"
#include "../Image/UIImage.h"
#include "../Image/UI9PatchImage.h"
#include "../Util/IUIInteractable.h"
#include "../Text/UIText.h"
#include "../VisualStyle.h"
#include <utility>
#include <vector>
#include <functional>


class UIImageTextButton : public UIContainerBase, public IUIInteractable {
private:
    std::vector<UIImage*> images;
    std::vector<UI9PatchImage*> patchImages;
    std::vector<UIText*> texts;
    std::function<void()> onClick = nullptr;
    
public:

    void AddImage(const ImageStyle& imageStyle, const D2D1_RECT_F& rect)
    {
        auto* img = new UIImage();
        img->Init(rect);
        img->SetStyle(imageStyle);
        AddChild(img);
        images.push_back(img);
    }
    void AddImage(const NinePatchStyle& imageStyle, const D2D1_RECT_F& rect)
    {
        auto* img = new UI9PatchImage();
        img->Init(rect);
        img->SetStyle(imageStyle);
        AddChild(img);
        patchImages.push_back(img);
    }
    
    void AddText(const std::wstring& content, const TextStyle& style, const D2D1_RECT_F& layout) {
        auto* txt = new UIText();
        txt->Init(style, content, layout);
        AddChild(txt);
        texts.push_back(txt);
    }

    void SetOnClick(std::function<void()> fn = nullptr) {
        onClick = std::move(fn);
    }

    const std::vector<UIText*>& GetTexts() const {
        return texts;
    }
    const std::vector<UIImage*>& GetImages() const
    {
        return images;
    }
    const std::vector<UI9PatchImage*>& GetPatchImages() const
    {
        return patchImages;
    }

    // All Child Crear (delete)
    void Clear()
    {
        images.clear();
        patchImages.clear();
        texts.clear();
        ClearChild();
    }

    // 조합
    
    void InitFromStyle(const UIIconStyle& style, const D2D1_RECT_F& layout) {
        SetRect(layout);

        auto childLayout = D2D1_RECT_F{0,0, GetWidth(), GetHeight()};
        
        // 아이콘 이미지
        if (style.bgStyle.image) {
            AddImage(style.bgStyle, childLayout);
        }
        if (style.iconStyle.image) {
            AddImage(style.iconStyle, childLayout); // iconStyle.padding은 UIImage 내부에서 처리됨
        }
    }

    void InitFromStyle(const UIInventorySlotStyle& style, const D2D1_RECT_F& layout) {
        SetRect(layout);
        
        auto childLayout = D2D1_RECT_F{0,0, GetWidth(), GetHeight()};
        
        // 아이콘 이미지
        if (style.background.image) {
            AddImage(style.background, childLayout);
        }
        if (style.itemIcon.image) {
            AddImage(style.itemIcon, childLayout); // iconStyle.padding은 UIImage 내부에서 처리됨
        }

        AddText(L"", style.quantityTextStyle, childLayout);
        AddText(L"", style.enhancementTextStyle, childLayout);
    }

    bool HandleClick(int x, int y) override {
        auto r = GetScaledDrawRect();

        if (x >= r.left && x <= r.right &&
            y >= r.top && y <= r.bottom)
        {
            if (onClick) 
            {
                onClick();
                return true;
            }
        }
        return false;
    }
};
