#include "stdafx.h"
#include "GameScene.h"
#include "BackGround.h"
#include "Player.h"
#include "EnemyMissile.h"
#include "BossTank.h"
#include "StepObject.h"

namespace
{
	const Vector3 BOSS_POSITION = { 0.0f,0.0f,-1500.0f };
	const Vector3 PLAYER_START_POSITION = { 0.0f,0.0f,1200.0f };
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
		m_player->SetPosition(PLAYER_START_POSITION);

		m_bossTank = NewGO<BossTank>(0,"bosstank");
		m_bossTank->SetPosition(BOSS_POSITION);

		StepObject* stepObject = NewGO<StepObject>(0, "targetObject");
		stepObject->SetPosition({ 0.0f,200.0f,-200.0f });

		stepObject = NewGO<StepObject>(0, "targetObject");
		stepObject->SetPosition({ 100.0f,300.0f,-700.0f });

		return true;
	}

	void GameScene::Update()
	{
		
	}
}