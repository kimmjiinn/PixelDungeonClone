#pragma once
#include "../../D2DImageManager.h"
#include "../../DWriteFactory.h"

class UIResourceSubManager {
public:
    // 각 UI별 리소스 프리로드 함수
    static void Preload_StatusToolbar();
    static void Preload_QuickSlotToolbar();
    static void Preload_TopRightUI();
    static void Preload_InventoryUI();
    static void Preload_NinePatch();
    static void PreloadAll();            // 💡 전체 리소스 한 번에 불러오기
};

inline void InitFontResources() {
    auto* factory = DWriteFactory::GetInstance();

    // 메인 폰트
    factory->AddFont(L"pixel", 14);
    factory->AddFont(L"pixel", 20);
    factory->AddFont(L"pixel", 32);
    factory->AddFont(L"pixel", 48);
}