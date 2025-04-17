#pragma once
#include "Singleton.h"
#include "config.h"

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

public:
    void Init();
    void Release();

    D2DImage* AddImage(string key, const wchar_t* filePath);

    D2DImage* AddImage(string key, const wchar_t* filePath, int maxFrameX, int maxFrameY);

    void DeleteImage(string key);
    D2DImage* FindImage(string key);
};

