#include "AstarTile.h"
#include "Image.h"

HRESULT AstarTile::Init()
{
    return E_NOTIMPL;
}

HRESULT AstarTile::Init(int idX, int idY)
{
    // 타일 위치 및 크기 초기화
    this->idX = idX;
    this->idY = idY;

    // 타일 중심점 계산
    center.x = idX * 30 + 15;  // 30은 타일 크기, 15는 타일 크기의 절반
    center.y = idY * 30 + 15;

    // 타일 영역 계산
    rc.left = idX * 30;
    rc.right = rc.left + 30;
    rc.top = idY * 30;
    rc.bottom = rc.top + 30;

    // A* 알고리즘 비용 초기화
    costFromStart = 0.0f;
    costToGoal = 0.0f;
    totalCost = 0.0f;

    // 타일 타입 초기화
    type = TileType::None;

    // 부모 타일 초기화
    parentTile = nullptr;

    // 타일 색상 초기화
    color = RGB(100, 100, 100);
    hBrush = CreateSolidBrush(color);

    return S_OK;
}

void AstarTile::Release()
{
    // 브러시 해제
    DeleteObject(hBrush);
}

void AstarTile::Update()
{
    // 현재는 업데이트할 내용 없음
}

void AstarTile::Render(HDC hdc)
{
    // 타일 이미지가 있으면 이미지 렌더링, 없으면 색상으로 렌더링
    if (tileImage)
    {
        tileImage->FrameRender(hdc, center.x, center.y, frameX, frameY, true);
    }
    else
    {
        hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
        FillRect(hdc, &rc, hBrush);
        Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
        SelectObject(hdc, hOldBrush);
    }
}

void AstarTile::SetColor(COLORREF color)
{
    this->color = color;
    
    // 기존 브러시 삭제 후 새 브러시 생성
    DeleteObject(hBrush);
    hBrush = CreateSolidBrush(color);
}
