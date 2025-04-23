#pragma once
#include "Singleton.h"
#include "config.h"
#include "D2DTypes.h"

///*
//	배열 : 인덱스 -> 데이터
//	stl::vector : 인덱스 -> 데이터
//	std::map : 키 -> 데이터
//
//	레드-블랙 트리 구현 : 삽입, 삭제, 검색 O(logN) 시간복잡도
//*/

class D2DImage;
class D2DImageManager : public Singleton<D2DImageManager>
{
private:
    map<string, D2DImage*> mapImages;
    std::map<std::string, D2DImageFrame> frameMap;

public:
    void Init();
    void Release();

    D2DImage* AddImage(string key, const wchar_t* filePath);

    D2DImage* AddImage(string key, const wchar_t* filePath, int maxFrameX, int maxFrameY);

    void DeleteImage(string key);
    D2DImage* FindImage(string key);

    D2DImage* CreateImage(const wchar_t* filePath);
    D2DImage* CreaetImage(const wchar_t* filePath, int maxFrameX, int maxFrameY);
    void RegisterFrame(std::string key, const std::string& baseImageKey, D2D1_RECT_F rect);
    D2DImageFrame* FindFrame(std::string key);

    inline D2D1_RECT_F MakeRectTopLeft(FPOINT pos, float width, float height) {
        return D2D1::RectF(pos.x, pos.y, pos.x + width, pos.y + height);
    }
    inline D2D1_RECT_F MakeRectCenter(FPOINT center, float width, float height) {
        return D2D1::RectF(center.x - width / 2, center.y - height / 2,
            center.x + width / 2, center.y + height / 2);
    }

};

