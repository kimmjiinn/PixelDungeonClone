#include "MainGame.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Timer.h"
#include "BattleScene.h"
#include "TilemapTool.h"
#include "AstarScene.h"
#include "LoadingScene.h"
#include "D2DImage.h"
#include "D2DImageManager.h"

HRESULT MainGame::Init()
{
	ImageManager::GetInstance()->Init();
	KeyManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init();

	SceneManager::GetInstance()->AddScene("A*알고리즘", new AstarScene());
	SceneManager::GetInstance()->AddScene("전투씬_1", new BattleScene());
	SceneManager::GetInstance()->AddScene("타일맵툴", new TilemapTool());
	SceneManager::GetInstance()->AddLoadingScene("로딩_1", new LoadingScene());
	SceneManager::GetInstance()->ChangeScene("A*알고리즘");

	hdc = GetDC(g_hWnd);

	backBuffer = new Image();
	if (FAILED(backBuffer->Init(TILEMAPTOOL_X, TILEMAPTOOL_Y)))
	{
		MessageBox(g_hWnd, 
			TEXT("백버퍼 생성 실패"), TEXT("경고"), MB_OK);
		return E_FAIL;
	}

	test = D2DImageManager::GetInstance()->AddImage("banner", L"Image/banners.png", 2, 4);
	// test = new D2DImage();
	// test->LoadFromFile(L"Image/banners.png", 2, 4);
	
	return S_OK;
}

void MainGame::Release()
{
	if (backBuffer)
	{
		backBuffer->Release();
		delete backBuffer;
		backBuffer = nullptr;
	}

	// if (test)
	// {
	// 	test->Release();
	// 	delete test;
	// 	test = nullptr;
	// }
	
	ReleaseDC(g_hWnd, hdc);

	SceneManager::GetInstance()->Release();
	KeyManager::GetInstance()->Release();
	ImageManager::GetInstance()->Release();
}

void MainGame::Update()
{
	SceneManager::GetInstance()->Update();
	InvalidateRect(g_hWnd, NULL, false);
}

void MainGame::Render()
{
	D2DImage::BeginDraw();
	D2DImage::Clear(D2D1::ColorF(D2D1::ColorF::Black));

	test->Middle_RenderFrame(WINSIZE_X/2, WINSIZE_Y/2, 0, 3, DEG_TO_RAD(135),false,false,0.5f);
	
	// // 백버퍼에 먼저 복사
	// HDC hBackBufferDC = backBuffer->GetMemDC();
	//
	// SceneManager::GetInstance()->Render(hBackBufferDC);
	//
	// TimerManager::GetInstance()->Render(hBackBufferDC);
	// wsprintf(szText, TEXT("Mouse X : %d, Y : %d"), g_ptMouse.x, g_ptMouse.y);
	// TextOut(hBackBufferDC, 20, 60, szText, wcslen(szText));
	//
	// // 백버퍼에 있는 내용을 메인 hdc에 복사
	// backBuffer->Render(hdc);
	D2DImage::EndDraw();
}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'a': case 'A':
			SceneManager::GetInstance()->ChangeScene("전투씬_1");
			break;
		case 'd': case 'D':
			SceneManager::GetInstance()->ChangeScene("타일맵툴");
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);

		break;
	case WM_LBUTTONUP:
		break;
	case WM_MOUSEMOVE:
		g_ptMouse.x = LOWORD(lParam);
		g_ptMouse.y = HIWORD(lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

MainGame::MainGame()
{
}

MainGame::~MainGame()
{
}
