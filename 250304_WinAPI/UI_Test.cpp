#include "UI_Test.h"
#include "D2DImage.h"
#include "CommonFunction.h"
#include "config.h"
#include "UI/Test/UITestView.h"

HRESULT UI_TestScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);

	testView = new UITestView();

	testView->Init();
    return S_OK;
}

void UI_TestScene::Release()
{
	testView->Release();
}

void UI_TestScene::Update()
{
	testView->Update(0.1f);
}

void UI_TestScene::Render()
{
	testView->Render(D2DImage::GetRenderTarget());
}
