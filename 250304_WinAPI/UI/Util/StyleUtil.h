#pragma once
#include "../VisualStyle.h"

namespace StyleUtil {

    inline std::shared_ptr<ImageStyle> CloneImageStyle(const std::shared_ptr<ImageStyle>& src) {
        return src ? std::make_shared<ImageStyle>(*src) : nullptr;
    }

    inline std::shared_ptr<TextStyle> CloneTextStyle(const std::shared_ptr<TextStyle>& src) {
        return src ? std::make_shared<TextStyle>(*src) : nullptr;
    }

    inline BarStyle CloneBarStyle(const BarStyle& src) {
        return {
            // CloneImageStyle(src.background),
            // CloneImageStyle(src.fill),
            // CloneTextStyle(src.infoText)
        };
    }

    inline UIInventorySlotStyle CloneInventorySlotStyle(const UIInventorySlotStyle& src) {
        return {
            // CloneImageStyle(src.background),
            // CloneImageStyle(src.itemIcon),
            // CloneTextStyle(src.quantityTextStyle),
            // CloneTextStyle(src.enhancementTextStyle)
        };
    }

    inline UIIconStyle CloneIconStyle(const UIIconStyle& src) {
        return {
            // CloneImageStyle(src.bgStyle),
            // CloneImageStyle(src.iconStyle)
        };
    }

    inline UIButtonStyle CloneButtonStyle(const UIButtonStyle& src) {
        return {
            // CloneImageStyle(src.background),
            // CloneTextStyle(src.textStyle)
        };
    }

    inline NinePatchStyle CloneNinePatchStyle(const NinePatchStyle& src) {
        return {
            // CloneImageStyle(src.topLeft), CloneImageStyle(src.top), CloneImageStyle(src.topRight),
            // CloneImageStyle(src.left), CloneImageStyle(src.center), CloneImageStyle(src.right),
            // CloneImageStyle(src.bottomLeft), CloneImageStyle(src.bottom), CloneImageStyle(src.bottomRight),
            // src.cornerSize
        };
    }
}