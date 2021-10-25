#include "stdafx.h"
#include "GameScene.h"
#include "BackGround.h"
#include "Player.h"
#include "Enemy.h"

namespace
{
	const Vector3 ENEMY_POSITION = { 200.0f,0.0f,200.0f };
}

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
		m_enemy = NewGO<Enemy>(0, "enemy");
		m_enemy->SetPosition(ENEMY_POSITION);
		return true;
	}

	void GameScene::Update()
	{
		
	}
}