#include "stdafx.h"
#include "GameScene.h"
#include "BackGround.h"
#include "Player.h"
#include "EnemyMissile.h"
#include "BossTank.h"

namespace
{
	const Vector3 ENEMY_POSITION[4] = { { 200.0f,150.0f,500.0f }, { -300.0f,150.0f,200.0f } ,{ -100.0f,150.0f,-300.0f } ,{ 200.0f,0.0f,-200.0f } };
	const Vector3 BOSS_POSITION = { 0.0f,0.0f,-1500.0f };
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
			m_enemy[i] = NewGO<EnemyMissile>(0, "enemy");
			m_enemy[i]->SetPosition(ENEMY_POSITION[i]);
		}

		m_bossTank = NewGO<BossTank>(0,"bosstank");
		m_bossTank->SetPosition(BOSS_POSITION);

		//NOTE:テスト用
		Quaternion testRotX;
		testRotX.SetRotationDegX(50.0f);
		m_enemy[2]->SetRotation(testRotX);

		Quaternion testRotZ;
		testRotZ.SetRotationDegZ(60.0f);
		m_enemy[1]->SetRotation(testRotZ);

		Quaternion testRotY;
		testRotY.SetRotationY(79.0f);
		testRotX.Multiply(testRotZ);
		testRotX.Multiply(testRotY);;
		m_enemy[0]->SetRotation(testRotX);
		return true;
	}

	void GameScene::Update()
	{
		
	}
}