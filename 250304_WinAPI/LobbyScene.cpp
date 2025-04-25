#include "LobbyScene.h"
#include"CommonFunction.h"
#include "D2DImageManager.h"
#include "D2DImage.h"
#include "SceneManager.h"
#include "MouseManager.h"
#include "KeyManager.h"
#include "FModSoundPlayer.h"
#include "LobbyButton.h"
#include "LobbyBackground.h"
#include "TimerManager.h"

HRESULT LobbyScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, WINSIZE_Y);
	title = D2DImageManager::GetInstance()->AddImage("로비타이틀", L"Image/LobbyTitle.png");
	fireball = D2DImageManager::GetInstance()->AddImage("파이어볼", L"Image/fireballSheet.png", 20, 1);
	
	elapsedTime = 0.0f;
	curFrame = 0;

	FModSoundPlayer::GetInstance()->Play("theme");

	background = new LobbyBackground;
	background->Init();

	mainMapButton = new MapButton;
	toolMapButton = new MapButton;
	exitButton = new ExitButton;
	mainMapTXT = D2DImageManager::GetInstance()->AddImage("메인맵텍스트", L"Image/playtxt.png");
	toolMapTXT = D2DImageManager::GetInstance()->AddImage("툴맵텍스트", L"Image/tooltxt.png");
	team2TXT = D2DImageManager::GetInstance()->AddImage("팀2텍스트", L"Image/Team2txt.png");

	// 버튼 영역
	dashboardRc.left = 216 * 2;
	dashboardRc.top = WINSIZE_Y / 2 - 70;
	dashboardRc.right = 216 * 3;
	dashboardRc.bottom = WINSIZE_Y / 2 + 150;

	mainMapButton->Init(dashboardRc.left, dashboardRc.top);
	mainMapButton->SetFunction([]() {
		FModSoundPlayer::GetInstance()->Stop("theme");
		SceneManager::GetInstance()->ChangeScene("게임씬");
		});

	toolMapButton->Init(dashboardRc.right-32*3, dashboardRc.top);
	toolMapButton->SetFunction([]()
		{
			FModSoundPlayer::GetInstance()->Stop("theme");
			SceneManager::GetInstance()->ChangeScene("타일맵툴");
		});

	exitButton->Init(WINSIZE_X - 30, 10);
	exitButton->SetFunction([]()
		{
			// 끝내기
			PostQuitMessage(0);
		});

	// 메인
	mainRc.left = dashboardRc.left;
	mainRc.top = dashboardRc.top;
	mainRc.right = mainRc.left + (32 * 3);
	mainRc.bottom = mainRc.top + (32 * 3);
	
	// 툴
	toolRc.right = dashboardRc.right;
	toolRc.top = dashboardRc.top;
	toolRc.left = toolRc.right- (32 * 3);
	toolRc.bottom = toolRc.top + (32 * 3);
	return S_OK;
}

void LobbyScene::Release()
{
}

void LobbyScene::Update()
{
	mainMapButton->Update();
	toolMapButton->Update();
	exitButton->Update();

	background->Update();

	// 애니메이션
	float deltaTime = TimerManager::GetInstance()->GetDeltaTime();
	elapsedTime += deltaTime;
	if (elapsedTime >= 0.1f)
	{
		elapsedTime = 0;
		++curFrame;
		if (curFrame >= fireball->GetMaxFrameX())
		{
				curFrame = 0;
		}
	}
}

void LobbyScene::Render(HDC hdc)
{
	background->Render(hdc);

	title->Middle_RenderFrameScale(WINSIZE_X / 2, WINSIZE_Y / 4, 1.5, 1.5, 0, 0);

	fireball->Middle_RenderFrameScale(WINSIZE_X / 2 - (title->GetWidth() / 2), WINSIZE_Y / 4 - (title->GetHeight() / 4), 3, 3, curFrame, 0);
	fireball->Middle_RenderFrameScale(WINSIZE_X / 2 + (title->GetWidth() / 2), WINSIZE_Y / 4 - (title->GetHeight() / 4), 3, 3, curFrame, 0);

	mainMapButton->Render(hdc);
	toolMapButton->Render(hdc);
	exitButton->Render(hdc);

	//RenderRect(hdc, dashboardRc);
	//RenderRect(hdc, mainRc);
	//RenderRect(hdc, toolRc);
	mainMapTXT->RenderFrameScale((mainRc.left + 16), mainRc.bottom, 3, 3, 0, 0);

	toolMapTXT->RenderFrameScale((toolRc.left + 16), toolRc.bottom, 3, 3, 0, 0);

	team2TXT->Middle_RenderFrameScale(WINSIZE_X - 50, WINSIZE_Y - 30, 2, 2, 0, 0);
}

LobbyScene::LobbyScene()
{
}
