#include "BattleScene.h"
#include "Image.h"
#include "CommonFunction.h"
#include "config.h"
#include "Player.h"

HRESULT BattleScene::Init()
{
	SetClientRect(g_hWnd, WINSIZE_X, TILEMAPTOOL_Y);

	player = new Player({0,0});
	player->Init();
	
	sampleTile = ImageManager::GetInstance()->AddImage(
		"��Ʋ��Ƽ_����Ÿ��", L"Image/mapTiles.bmp", 640, 288,
		SAMPLE_TILE_X, SAMPLE_TILE_Y);

	backGround = new Image();
	if (FAILED(backGround->Init(TEXT("Image/BackGround.bmp"), WINSIZE_X, TILEMAPTOOL_Y)))
	{
		MessageBox(g_hWnd,
			TEXT("Image/backGround.bmp ���� ����"), TEXT("���"), MB_OK);
		return E_FAIL;
	}
	
	Load();


	
	return S_OK;
}

void BattleScene::Release()
{
	//if (backGround)
	//{
	//	backGround->Release();
	//	delete backGround;
	//	backGround = nullptr;
	//}
	SAFE_DELETE(backGround);
}

void BattleScene::Update()
{
	// for (auto actor : actors)
	// {
	// 	actor->gainEnergy(actor->speed);
	// 	if (actor->hasEnoughEnergy())
	// 	{
	// 		auto action = actor->takeTurn();
	// 		action->perform();
	// 	}
	// }

	
	// if (KeyManager::GetInstance()->IsOnceKeyDown(VK_RETURN))
	// {
	// 	SceneManager::GetInstance()->ChangeScene("������_1", "�ε�_1");
	// }
	player->Update();
}

void BattleScene::Render(HDC hdc)
{
	backGround->Render(hdc);
	// ���� Ÿ�� ����
	for (int i = 0; i < TILE_X * TILE_Y; ++i)
	{
		sampleTile->FrameRender(hdc, tileInfo[i].rc.left,
			tileInfo[i].rc.top, tileInfo[i].frameX,
			tileInfo[i].frameY, false, false);
	}
	player->Render(hdc);
}

void BattleScene::Load()
{
	// ���� �ε�
	HANDLE hFile = CreateFile(
		L"TileMapData.dat", GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(g_hWnd, TEXT("���� ���� ����"), TEXT("���"), MB_OK);
		return;
	}
	DWORD dwByte = 0;
	if (!ReadFile(hFile, tileInfo, sizeof(tileInfo), &dwByte, NULL))
	{
		MessageBox(g_hWnd, TEXT("���� �б� ����"), TEXT("���"), MB_OK);
	}
	CloseHandle(hFile);
}
