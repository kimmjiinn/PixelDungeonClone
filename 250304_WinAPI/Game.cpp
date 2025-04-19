#include "Game.h"
#include  "BattleScene.h"
#include  "Entity.h"
#include "Player.h"
#include  "TurnManager.h"

Game::Game()
{
	map = new BattleScene;
	player = new Player;
}

Game::~Game()
{
	if(map)
	{
		delete map;
		map = nullptr;
	}
	if (player)
	{
		delete player;
		player = nullptr;
	}
}

void Game::Update()
{
	player->Update();
}

void Game::Render(HDC hdc)
{
	player->Render(hdc);
}

void Game::AddActor(Entity* actor)
{
}

void Game::HandleInput(WPARAM wParam)
{
	switch (wParam)
	{
	case 'a': case 'A':
		player->Move(-30, 0, *map);
		break;
	case 'd': case 'D':
		//SceneManager::GetInstance()->ChangeScene("타일맵툴");
		player->Move(30, 0, *map);
		break;
	case 'w': case 'W':
		player->Move(0, -30, *map);
		break;
	case 's': case 'S':
		player->Move(0, 30, *map);
		break;
	}
}
