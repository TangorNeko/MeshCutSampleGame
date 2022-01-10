#include "stdafx.h"
#include "GameScene.h"
#include "BackGround.h"
#include "Player.h"
#include "EnemyMissile.h"
#include "BossTank.h"
#include "HealItem.h"
#include "EnemyRock.h"
#include "StepObject.h"
#include "CutDummy.h"
#include "MiniEnemy.h"

namespace
{
	const Vector3 BOSS_POSITION = { 0.0f,0.0f,-1300.0f };
	const Vector3 PLAYER_START_POSITION = { 0.0f,30.0f,2000.0f };

	const Vector3 MINION_POSITION_PHASE1 = { 0.0f,0.0f,-500.0f };

	const int SUMMON_NUM_PHASE2 = 2;
	const Vector3 MINION_POSITIONS_PHASE2[SUMMON_NUM_PHASE2] = { {-400.0f,0.0f,-800.0f}, {400.0f,0.0f,-800.0f} };

	const int SUMMON_NUM_PHASE3 = 4;
	const Vector3 MINION_POSITIONS_PHASE3[SUMMON_NUM_PHASE3] = { { 850.0f,0.0f,-1400.0f }, { -850.0f,0.0f,-1400.0f } ,{ 1300.0f,0.0f,-1000.0f } ,{ -1300.0f,0.0f,-1000.0f } };

	const int PHASE_WAIT_TIME = 180;
}

namespace Game
{
	GameScene::~GameScene()
	{
		DeleteGO(m_backGround);
		DeleteGO(m_player);
		DeleteGO(m_bossTank);
		DeleteProducts();
	}

	bool GameScene::Start()
	{
		m_backGround = NewGO<BackGround>(0,"background");
		m_player = NewGO<Player>(0, "player");
		m_player->SetPosition(PLAYER_START_POSITION);
		return true;
	}

	void GameScene::Update()
	{
		Vector3 toTrigger = m_player->GetPosition();
		toTrigger -= {0.0f, 0.0f, 1400.0f};

		if (toTrigger.LengthSq() < 300.0f * 300.0f && m_phase == 0)
		{
			m_phase = 1;
			m_phaseWaitTime = 0;
		}

		if(m_phase == 1 && m_phaseWaitTime == PHASE_WAIT_TIME)
		{
			MiniEnemy* enemy = NewGO<MiniEnemy>(0, "enemy");
			enemy->SetPosition(MINION_POSITION_PHASE1);

			Game::Effect* spawnEffect = NewGO<Game::Effect>(1);
			spawnEffect->SetPosition(MINION_POSITION_PHASE1);
			spawnEffect->Init(u"Assets/effect/Teleport.efk");
			spawnEffect->Play();


			m_phase = 2;

		}


		MiniEnemy* mini = FindGO<MiniEnemy>("enemy");


		if (mini == nullptr && m_phase == 6)
		{
			m_phase = 7;
			m_phaseWaitTime = 0;
		}

		if(m_phase == 7 && m_phaseWaitTime == PHASE_WAIT_TIME)
		{
			m_bossTank = NewGO<BossTank>(0, "bosstank");
			m_bossTank->SetPosition(BOSS_POSITION);

			Game::Effect* spawnEffect = NewGO<Game::Effect>(1);
			spawnEffect->Init(u"Assets/effect/Teleport.efk");
			spawnEffect->SetPosition(BOSS_POSITION);
			spawnEffect->Play();

			m_phase = 8;
		}


		if (mini == nullptr && m_phase == 4)
		{
			m_phase = 5;
			m_phaseWaitTime = 0;
		}

		if(m_phase == 5 && m_phaseWaitTime == PHASE_WAIT_TIME)
		{
			MiniEnemy* enemy[SUMMON_NUM_PHASE3];
			for (int i = 0; i < SUMMON_NUM_PHASE3; i++)
			{
				enemy[i] = NewGO<MiniEnemy>(0, "enemy");
				enemy[i]->SetPosition(MINION_POSITIONS_PHASE3[i]);

				Game::Effect* spawnEffect = NewGO<Game::Effect>(1);
				spawnEffect->Init(u"Assets/effect/Teleport.efk");
				spawnEffect->SetPosition(MINION_POSITIONS_PHASE3[i]);
				spawnEffect->Play();
			}
			
			m_phase = 6;
		}


		if (mini == nullptr && m_phase == 2)
		{
			m_phase = 3;
			m_phaseWaitTime = 0;
		}

		if(m_phase == 3 && m_phaseWaitTime == PHASE_WAIT_TIME)
		{
			MiniEnemy* enemy[SUMMON_NUM_PHASE2];
			for (int i = 0; i < SUMMON_NUM_PHASE2; i++)
			{
				enemy[i] = NewGO<MiniEnemy>(0, "enemy");
				enemy[i]->SetPosition(MINION_POSITIONS_PHASE2[i]);

				Game::Effect* spawnEffect = NewGO<Game::Effect>(0);
				spawnEffect->Init(u"Assets/effect/Teleport.efk");
				spawnEffect->SetPosition(MINION_POSITIONS_PHASE2[i]);
				spawnEffect->Play();
			}

			m_phase = 4;
		}

		m_phaseWaitTime++;

		if (g_pad[0]->IsTrigger(enButtonA))
		{
			Effect* ef = NewGO<Effect>(0);
			ef->Init(u"Assets/effect/Warning.efk");
			ef->SetPosition(PLAYER_START_POSITION);
			ef->Play();
		}
	}

	//TODO:GameScene以外で生成している物は専用の削除クラスがあったほうがいいかも。
	void GameScene::DeleteProducts()
	{
		QueryGOs<EnemyMissile>("missile", [](EnemyMissile* enemyMissile)->bool
			{
				DeleteGO(enemyMissile);
				return true;
			}
		);

		QueryGOs<HealItem>("healItem", [](HealItem* healItem)->bool
			{
				DeleteGO(healItem);
				return true;
			}
		);

		QueryGOs<EnemyRock>("rock", [](EnemyRock* enemyRock)->bool
			{
				DeleteGO(enemyRock);
				return true;
			}
		);

		QueryGOs<StepObject>("stepObject", [](StepObject* stepObject)->bool
			{
				DeleteGO(stepObject);
				return true;
			}
		);

		QueryGOs<CutDummy>("cutDummy", [](CutDummy* cutDummy)->bool
			{
				DeleteGO(cutDummy);
				return true;
			}
		);

		EffectEngine::GetInstance()->StopAllEffects();
	}
}