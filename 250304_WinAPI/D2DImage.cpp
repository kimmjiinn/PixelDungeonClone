#include "D2DImage.h"

ID2D1Factory* D2DImage::factory = nullptr;
ID2D1HwndRenderTarget* D2DImage::renderTarget = nullptr;
ID2D1SolidColorBrush* D2DImage::brushes[5] = { nullptr, nullptr, nullptr, nullptr, nullptr };

#pragma region D2D_LifeCycle
HRESULT D2DImage::InitD2D(HWND hWnd) {
    if (!factory) {
        D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
    }

    RECT rc;
    GetClientRect(hWnd, &rc);

    if (!renderTarget) {
        D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
        D2D1_RENDER_TARGET_PROPERTIES rtProps = D2D1::RenderTargetProperties();
        D2D1_HWND_RENDER_TARGET_PROPERTIES hwndProps = D2D1::HwndRenderTargetProperties(hWnd, size);
        HRESULT hr = factory->CreateHwndRenderTarget(rtProps, hwndProps, &renderTarget);
        
        renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &brushes[0]);
        renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &brushes[1]);
        renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &brushes[2]);
        renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brushes[3]);
        renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brushes[4]);

        return hr;
    }

    
    return S_OK;
}

HRESULT D2DImage::BeginDraw() {
    if (!renderTarget) return E_FAIL;
    renderTarget->BeginDraw();
    return S_OK;
}

HRESULT D2DImage::EndDraw() {
    if (!renderTarget) return E_FAIL;
    return renderTarget->EndDraw();
}

void D2DImage::Clear(D2D1::ColorF color) {
    if (renderTarget) renderTarget->Clear(color);
}

ID2D1HwndRenderTarget* D2DImage::GetRenderTarget() {
    return renderTarget;
}
#pragma endregion

HRESULT D2DImage::LoadFromFile(const wchar_t* filePath, int maxFrameX, int maxFrameY) {
    IWICImagingFactory* wicFactory = nullptr;
    IWICBitmapDecoder* decoder = nullptr;
    IWICBitmapFrameDecode* frame = nullptr;
    IWICFormatConverter* converter = nullptr;

    HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&wicFactory));
    if (FAILED(hr)) return hr;

    hr = wicFactory->CreateDecoderFromFilename(filePath, NULL, GENERIC_READ,
        WICDecodeMetadataCacheOnLoad, &decoder);
    if (FAILED(hr)) return hr;

    hr = decoder->GetFrame(0, &frame);
    if (FAILED(hr)) return hr;

    hr = wicFactory->CreateFormatConverter(&converter);
    if (FAILED(hr)) return hr;

    hr = converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeCustom);
    if (FAILED(hr)) return hr;

    hr = renderTarget->CreateBitmapFromWicBitmap(converter, NULL, &bitmap);

    if (converter) converter->Release();
    if (frame) frame->Release();
    if (decoder) decoder->Release();
    if (wicFactory) wicFactory->Release();
    if (FAILED(hr)) return hr;
    if (!bitmap) return E_FAIL;
    D2D1_SIZE_F size = bitmap->GetSize();

    this->maxFrameX = maxFrameX;
    this->maxFrameY = maxFrameY;
    this->frameWidth = static_cast<int>(size.width) / maxFrameX;
    this->frameHeight = static_cast<int>(size.height) / maxFrameY;

    return S_OK;
}

void D2DImage::Middle_Render(float x, float y, float angle, 
    bool flipX, bool flipY, float alpha) {
    if (!bitmap || !renderTarget) return;

    D2D1_SIZE_F bmpSize = bitmap->GetSize();
    D2D1_RECT_F destRect = D2D1::RectF(x - bmpSize.width / 2, y - bmpSize.height / 2, x + bmpSize.width/2, y + bmpSize.height/2);

    D2D1::Matrix3x2F transform = D2D1::Matrix3x2F::Identity();
    transform = transform * D2D1::Matrix3x2F::Translation(-bmpSize.width / 2.0f, -bmpSize.height / 2.0f);
    transform = transform * D2D1::Matrix3x2F::Scale(flipX ? -1.0f : 1.0f, flipY ? -1.0f : 1.0f);
    transform = transform * D2D1::Matrix3x2F::Rotation(angle);
    transform = transform * D2D1::Matrix3x2F::Translation(x + bmpSize.width / 2.0f, y + bmpSize.height / 2.0f);
    //transform = transform * D2D1::Matrix3x2F::Translation(x, y);
    renderTarget->SetTransform(transform);
    renderTarget->DrawBitmap(bitmap, D2D1::RectF(0, 0, bmpSize.width, bmpSize.height), alpha);
    renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void D2DImage::Middle_RenderFrame(float x, float y, int frameX, int frameY,
    float angle, bool flipX, bool flipY, float alpha) {
    if (!bitmap || !renderTarget || maxFrameX <= 0 || maxFrameY <= 0) return;
    
    int fx = frameX;
    int fy = frameY;

    D2D1_RECT_F srcRect = D2D1::RectF(
        static_cast<float>(fx * frameWidth),
        static_cast<float>(fy * frameHeight),
        static_cast<float>((fx + 1) * frameWidth),
        static_cast<float>((fy + 1) * frameHeight)
    );

    float halfWidth = frameWidth / 2.0f;
    float halfHeight = frameHeight / 2.0f;

    float centerX = x;
    float centerY = y;

    D2D1_RECT_F destRect = D2D1::RectF(
        centerX - halfWidth,
        centerY - halfHeight,
        centerX + halfWidth,
        centerY + halfHeight
    );

    D2D1::Matrix3x2F transform = D2D1::Matrix3x2F::Identity();
    transform = transform * D2D1::Matrix3x2F::Scale(
        flipX ? -1.0f : 1.0f,
        flipY ? -1.0f : 1.0f,
        D2D1::Point2F(centerX, centerY)
    );
    transform = transform * D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(centerX, centerY));

    renderTarget->SetTransform(transform);
    renderTarget->DrawBitmap(bitmap, destRect, alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srcRect);
    renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void D2DImage::Middle_RenderFrameScale(float x, float y, float scaleX, float scaleY, int frameX, int frameY, float angle, bool flipX, bool flipY, float alpha)
{
    if (!bitmap || !renderTarget || maxFrameX <= 0 || maxFrameY <= 0) return;

    int fx = frameX;
    int fy = frameY;

    D2D1_RECT_F srcRect = D2D1::RectF(
        static_cast<float>(fx * frameWidth),
        static_cast<float>(fy * frameHeight),
        static_cast<float>((fx + 1) * frameWidth),
        static_cast<float>((fy + 1) * frameHeight)
    );

    float halfWidth = frameWidth  / 2.0f;
    float halfHeight = frameHeight / 2.0f;

    float centerX = x;
    float centerY = y;

    D2D1_RECT_F destRect = D2D1::RectF(
        centerX - halfWidth,
        centerY - halfHeight,
        centerX + halfWidth,
        centerY + halfHeight
    );

    D2D1::Matrix3x2F transform = D2D1::Matrix3x2F::Identity();

    float finalScaleX = scaleX * (flipX ? -1.0f : 1.0f);
    float finalScaleY = scaleY * (flipY ? -1.0f : 1.0f);

    transform = transform * D2D1::Matrix3x2F::Scale(
        finalScaleX, finalScaleY,
        D2D1::Point2F(centerX, centerY)
    );

    transform = transform * D2D1::Matrix3x2F::Rotation(
        angle,
        D2D1::Point2F(centerX, centerY)
    );

    renderTarget->SetTransform(transform);
    renderTarget->DrawBitmap(bitmap, destRect, alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srcRect);
    renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void D2DImage::RenderPercent(FPOINT pos, float spercent, float epercent, float alpha)
{
    if (!bitmap || !renderTarget) return;

    D2D1_SIZE_F bmpSize = bitmap->GetSize();
    float sp = bmpSize.width * (spercent / 100.0f);
    float ep = bmpSize.width * (epercent / 100.0f);

    D2D1_RECT_F srcRect = D2D1::RectF(sp, 0, ep, bmpSize.height);

    D2D1_RECT_F destRect = D2D1::RectF(pos.x + sp, pos.y,
        pos.x + ep, pos.y + bmpSize.height);

    D2D1::Matrix3x2F transform = D2D1::Matrix3x2F::Identity();

    renderTarget->DrawBitmap(bitmap, destRect, alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRect);
    renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void D2DImage::RenderPercent(FPOINT pos, float spercent, float epercent, float scaleX, float scaleY, float angle, bool flipX, bool flipY, float alpha)
{
    if (!bitmap || !renderTarget) return;

    D2D1_SIZE_F bmpSize = bitmap->GetSize();
    float sp = bmpSize.width * (spercent / 100.0f);
    float ep = bmpSize.width * (epercent / 100.0f);

    D2D1_RECT_F srcRect = D2D1::RectF(sp, 0, ep, bmpSize.height);

    D2D1_RECT_F destRect = D2D1::RectF(pos.x + sp, pos.y,
        pos.x + ep, pos.y + bmpSize.height);

    D2D1::Matrix3x2F transform = D2D1::Matrix3x2F::Identity();

    float centerX = pos.x;
    float centerY = pos.y;

    float finalScaleX = scaleX * (flipX ? -1.0f : 1.0f);
    float finalScaleY = scaleY * (flipY ? -1.0f : 1.0f);

    transform = transform * D2D1::Matrix3x2F::Scale(
        finalScaleX, finalScaleY,
        D2D1::Point2F(centerX, centerY)
    );

    if (angle != 0.0f)
    {
        transform = transform * D2D1::Matrix3x2F::Rotation(
            angle,
            D2D1::Point2F(centerX, centerY)
        );
    }

    renderTarget->SetTransform(transform);
    renderTarget->DrawBitmap(bitmap, destRect, alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srcRect);
    renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void D2DImage::Render(float x, float y, float angle, bool flipX, bool flipY, float alpha)
{
    D2D1_SIZE_F bmpSize = bitmap->GetSize();
    Middle_Render(x + bmpSize.width / 2, y + bmpSize.height / 2, angle, flipX, flipY, alpha);
}

void D2DImage::RenderFrame(float x, float y, int frameX, int frameY, float angle, bool flipX, bool flipY, float alpha)
{
    Middle_RenderFrame(x + frameWidth / 2, y + frameHeight / 2, frameX, frameY, angle, flipX, flipY, alpha);
}

void D2DImage::RenderFrameScale(float x, float y, float scaleX, float scaleY, int frameX, int frameY, float angle, bool flipX, bool flipY, float alpha)
{
    if (!bitmap || !renderTarget || maxFrameX <= 0 || maxFrameY <= 0) return;

    int fx = frameX;
    int fy = frameY;

    // 🔸 1. 원본 프레임 영역 설정
    D2D1_RECT_F srcRect = D2D1::RectF(
        static_cast<float>(fx * frameWidth),
        static_cast<float>(fy * frameHeight),
        static_cast<float>((fx + 1) * frameWidth),
        static_cast<float>((fy + 1) * frameHeight)
    );

    float width = static_cast<float>(frameWidth);
    float height = static_cast<float>(frameHeight);

    // 🔸 2. 대상 렌더링 영역 (좌상단 기준)
    D2D1_RECT_F destRect = D2D1::RectF(
        x,
        y,
        x + width,
        y + height
    );

    // 🔸 3. 변환 행렬 계산 (좌상단 기준)
    float finalScaleX = scaleX * (flipX ? -1.0f : 1.0f);
    float finalScaleY = scaleY * (flipY ? -1.0f : 1.0f);

    D2D1::Matrix3x2F transform = D2D1::Matrix3x2F::Identity();

    // 👉 스케일 & 회전 기준점을 좌상단으로 지정
    transform = transform * D2D1::Matrix3x2F::Scale(
        finalScaleX, finalScaleY,
        D2D1::Point2F(x, y)
    );

    if (angle != 0.0f) {
        transform = transform * D2D1::Matrix3x2F::Rotation(
            angle,
            D2D1::Point2F(x, y)
        );
    }

    // 🔸 4. 렌더링
    renderTarget->SetTransform(transform);
    renderTarget->DrawBitmap(bitmap, destRect, alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRect);
    renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}


void D2DImage::InitBrushes() {
}
void D2DImage::ReleaseBrushes() {
    
}


void D2DImage::DrawLine(FPOINT point1, FPOINT point2, int color, float lineThickness)
{
    if (!renderTarget) return;
    D2D1_POINT_2F p1, p2;
    p1.x = point1.x;
    p1.y = point1.y;
    p2.x = point2.x;
    p2.y = point2.y;
    renderTarget->DrawLine(p1, p2, brushes[color], lineThickness);
}

void D2DImage::DrawRect(FPOINT leftUp, FPOINT rightDown, int color, float lineThickness)
{
    if (!renderTarget) return;
    D2D1_RECT_F rect;
    rect.left = leftUp.x;
    rect.top = leftUp.y;
    rect.right = rightDown.x;
    rect.bottom = rightDown.y;
    renderTarget->DrawRectangle(rect, brushes[color], lineThickness);
}

void D2DImage::DrawCircle(FPOINT center, float radius, int color, float lineThickness)
{
    if (!renderTarget) return;
    D2D1_POINT_2F c;
    c.x = center.x;
    c.y = center.y;
    D2D1_ELLIPSE ellipse = D2D1::Ellipse(c, radius, radius);
    renderTarget->DrawEllipse(ellipse, brushes[color], lineThickness);
}

void D2DImage::Middle_RenderRaw(ID2D1HwndRenderTarget* rt, const D2D1_RECT_F& destRect, const D2D1_RECT_F& srcRect,
    float scaleX, float scaleY, float angle, bool flipX, bool flipY, float alpha)
{
    if (!bitmap || !rt) return;

    float halfWidth = (destRect.right - destRect.left) * 0.5f;
    float halfHeight = (destRect.bottom - destRect.top) * 0.5f;
    float x = destRect.left - halfWidth;
    float y = destRect.top - halfHeight;


    float finalScaleX = scaleX * (flipX ? -1.0f : 1.0f);
    float finalScaleY = scaleY * (flipY ? -1.0f : 1.0f);

    D2D1::Matrix3x2F transform = D2D1::Matrix3x2F::Identity();

    // 스케일 적용
    transform = transform * D2D1::Matrix3x2F::Scale(
        finalScaleX,
        finalScaleY,
        D2D1::Point2F(x, y)
    );

    // 회전 적용
    if (angle != 0.0f) {
        transform = transform * D2D1::Matrix3x2F::Rotation(
            angle,
            D2D1::Point2F(x, y)
        );
    }

    rt->SetTransform(transform);
    rt->DrawBitmap(bitmap, destRect, alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srcRect);
    rt->SetTransform(D2D1::Matrix3x2F::Identity());
}

void D2DImage::RenderRaw(ID2D1HwndRenderTarget* rt, const D2D1_RECT_F& destRect, const D2D1_RECT_F& srcRect,
    float scaleX, float scaleY, float angle, bool flipX, bool flipY, float alpha)
{
    if (!bitmap || !rt) return;

    float x = destRect.left;
    float y = destRect.top;

    float finalScaleX = scaleX * (flipX ? -1.0f : 1.0f);
    float finalScaleY = scaleY * (flipY ? -1.0f : 1.0f);

    D2D1::Matrix3x2F transform = D2D1::Matrix3x2F::Identity();

    // 스케일 적용
    transform = transform * D2D1::Matrix3x2F::Scale(
        finalScaleX,
        finalScaleY,
        D2D1::Point2F(x, y)
    );

    // 회전 적용
    if (angle != 0.0f) {
        transform = transform * D2D1::Matrix3x2F::Rotation(
            angle,
            D2D1::Point2F(x, y)
        );
    }

    rt->SetTransform(transform);
    rt->DrawBitmap(bitmap, destRect, alpha, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srcRect);
    rt->SetTransform(D2D1::Matrix3x2F::Identity());
}

D2D1_RECT_F D2DImage::GetFullSourceRect() const {
    return D2D1::RectF(0, 0, static_cast<float>(GetWidth()), static_cast<float>(GetHeight()));
}

void D2DImage::Release() {

    if (bitmap) {
        //bitmap->Release();
        bitmap = nullptr;
    }

}

//프로그램이 종료될 때 단 한번만 호출
//factory가 static임
void D2DImage::ReleaseLast()
{
    if (factory) {
        factory->Release();
        factory = nullptr;
    }
    for (int i = 0; i < 5; ++i) {
        if (brushes[i]) {
            brushes[i]->Release();
            brushes[i] = nullptr;
        }
    }

    if (renderTarget) {
        renderTarget->Release();
        renderTarget = nullptr;
    }
}