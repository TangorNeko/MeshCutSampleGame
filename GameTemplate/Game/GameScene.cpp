#include "stdafx.h"
#include "GameScene.h"
#include "BackGround.h"
#include "Player.h"
#include "Enemy.h"

namespace
{
	const Vector3 ENEMY_POSITION[4] = { { 200.0f,0.0f,200.0f }, { 100.0f,0.0f,200.0f } ,{ -100.0f,150.0f,-300.0f } ,{ 200.0f,0.0f,-200.0f } };
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

		for (int i = 0; i < 4; i++)
		{
			m_enemy[i] = NewGO<Enemy>(0, "enemy");
			m_enemy[i]->SetPosition(ENEMY_POSITION[i]);
		}

		//NOTE:テスト用
		Quaternion testRot;
		testRot.SetRotationDegX(50.0f);
		m_enemy[2]->SetRotation(testRot);
		return true;
	}

	void GameScene::Update()
	{
		
	}
}