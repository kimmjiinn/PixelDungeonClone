#include "MainGame.h"
#include "CommonFunction.h"
#include "Image.h"
#include "Timer.h"
#include "BattleScene.h"
#include "TilemapTool.h"
#include "AstarScene.h"
#include "LoadingScene.h"
#include "DungeonScene.h"
#include "Camera.h"
#include "D2DImage.h"
#include "D2DImageManager.h"

HRESULT MainGame::Init()
{
	ImageManager::GetInstance()->Init();
	KeyManager::GetInstance()->Init();
	SceneManager::GetInstance()->Init();

	SceneManager::GetInstance()->AddScene("A*�˰���", new AstarScene());
	SceneManager::GetInstance()->AddScene("������_1", new BattleScene());
	SceneManager::GetInstance()->AddScene("Ÿ�ϸ���", new TilemapTool());
	SceneManager::GetInstance()->AddScene("�ȼ�����", new DungeonScene());
	SceneManager::GetInstance()->AddLoadingScene("�ε�_1", new LoadingScene());
	SceneManager::GetInstance()->ChangeScene("A*�˰���");

	hdc = GetDC(g_hWnd);

	backBuffer = new Image();
	if (FAILED(backBuffer->Init(TILEMAPTOOL_X, TILEMAPTOOL_Y)))
	{
		MessageBox(g_hWnd, 
			TEXT("����� ���� ����"), TEXT("���"), MB_OK);
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
	// D2DImage::BeginDraw();
	// D2DImage::Clear(D2D1::ColorF(D2D1::ColorF::Black));
	//
	// test->Middle_RenderFrameScale(WINSIZE_X/2, WINSIZE_Y/2, 9.f, 9.f, 0, 3, DEG_TO_RAD(0),false,false,0.5f);
	
	// ����ۿ� ���� ����
	HDC hBackBufferDC = backBuffer->GetMemDC();
	
	SceneManager::GetInstance()->Render(hBackBufferDC);
	
	TimerManager::GetInstance()->Render(hBackBufferDC);
	wsprintf(szText, TEXT("Mouse X : %d, Y : %d"), g_ptMouse.x, g_ptMouse.y);
	TextOut(hBackBufferDC, 20, 60, szText, wcslen(szText));
	
	// ����ۿ� �ִ� ������ ���� hdc�� ����
	backBuffer->Render(hdc);
	// D2DImage::EndDraw();
}

LRESULT MainGame::MainProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'a': case 'A':
			SceneManager::GetInstance()->ChangeScene("������_1");
			break;
		case 'd': case 'D':
			SceneManager::GetInstance()->ChangeScene("Ÿ�ϸ���");
			break;
		case 'p': case 'P':
			SceneManager::GetInstance()->ChangeScene("�ȼ�����");
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
	case WM_MOUSEWHEEL:
		// ���콺 �� �޽��� ó��
		{
			// zDelta �� ���� (�� ȸ�� ����� ��)
			int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
			
			// ī�޶� �ý��ۿ� ���콺 �� �Է� ����
			Camera::GetInstance()->HandleMouseWheel(zDelta);
		}
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
