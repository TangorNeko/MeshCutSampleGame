#include "stdafx.h"
#include "GameScene.h"
#include "BackGround.h"
#include "Player.h"

namespace Game
{
	GameScene::~GameScene()
	{
		DeleteGO(m_backGround);
	}

	bool GameScene::Start()
	{
		m_backGround = NewGO<BackGround>(0,"background");
		m_player = NewGO<Player>(0, "player");
		return true;
	}

	void GameScene::Update()
	{
		
	}
}