#include "D2DImageManager.h"
#include "D2DImage.h"

void D2DImageManager::Init()
{
}

void D2DImageManager::Release()
{
    map<string, D2DImage*>::iterator iter;
    for (iter = mapImages.begin(); iter != mapImages.end(); iter++)
    {
        if (iter->second)
        {
            (iter->second)->Release();
            delete (iter->second);
            (iter->second) = nullptr;
        }
    }
    mapImages.clear();

    ReleaseInstance();
}

D2DImage* D2DImageManager::AddImage(string key,
    const wchar_t* filePath)
{
    D2DImage* image = nullptr;
    image = FindImage(key);
    if (image)	return image;

    image = new D2DImage();
    if (FAILED(image->LoadFromFile(filePath)))
    {
        image->Release();
        delete image;

        return nullptr;
    }

    mapImages.insert(make_pair(key, image));
    return image;
}

D2DImage* D2DImageManager::AddImage(string key, 
    const wchar_t* filePath, int maxFrameX, int maxFrameY)
{
    D2DImage* image = nullptr;
    image = FindImage(key);
    if (image)	return image;

    image = new D2DImage();
    if (FAILED(image->LoadFromFile(filePath,maxFrameX, maxFrameY)))
    {
        image->Release();
        delete image;

        return nullptr;
    }

    mapImages.insert(make_pair(key, image));
    return image;
}

void D2DImageManager::DeleteImage(string key)
{
    map<string, D2DImage*>::iterator iter;
    iter = mapImages.find(key);

    if (iter == mapImages.end()) return;

    (iter->second)->Release();
    delete (iter->second); 
    (iter->second) = nullptr;

    mapImages.erase(iter);
}

D2DImage* D2DImageManager::FindImage(string key)
{
    map<string, D2DImage*>::iterator iter;
    iter = mapImages.find(key);

    if (iter == mapImages.end()) return nullptr;

    return iter->second;
}
