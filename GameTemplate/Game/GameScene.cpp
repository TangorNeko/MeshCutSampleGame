#include "stdafx.h"
#include "GameScene.h"
#include "BackGround.h"
#include "Player.h"
#include "BossTank.h"
#include "MiniEnemy.h"
#include "Fade.h"
#include "TitleScene.h"
#include "ProductsDeleter.h"
#include "DashTutorial.h"
#include "GameSceneBGM.h"

namespace
{
	const Vector3 BOSS_POSITION = { 0.0f,0.0f,-1300.0f };
	const Vector3 PLAYER_START_POSITION = { 0.0f,30.0f,2500.0f };

	const Vector3 MINION_POSITION_PHASE1 = { 0.0f,0.0f,-500.0f };

	const int SUMMON_NUM_PHASE2 = 2;
	const Vector3 MINION_POSITIONS_PHASE2[SUMMON_NUM_PHASE2] = { {-400.0f,0.0f,-800.0f}, {400.0f,0.0f,-800.0f} };

	const int SUMMON_NUM_PHASE3 = 4;
	const Vector3 MINION_POSITIONS_PHASE3[SUMMON_NUM_PHASE3] = { { 850.0f,0.0f,-1400.0f }, { -850.0f,0.0f,-1400.0f } ,{ 1300.0f,0.0f,-1000.0f } ,{ -1300.0f,0.0f,-1000.0f } };

	const int PHASE_WAIT_TIME = 180;

	const wchar_t* WARP_SOUND_PATH = L"Assets/sound/WarpSE.wav";
}

namespace Game
{
	GameScene::~GameScene()
	{
		DeleteGO(m_backGround);
		DeleteGO(m_player);
		DeleteGO(m_bossTank);
		DeleteGO(m_gameSceneBGM);
		DeleteProducts();
	}

	bool GameScene::Start()
	{
		m_backGround = NewGO<BackGround>(0,"background");
		m_player = NewGO<Player>(0, "player");
		m_player->SetPosition(PLAYER_START_POSITION);

		m_gameSceneBGM = NewGO<GameSceneBGM>(0, "gamescenebgm");
		return true;
	}

	void GameScene::Update()
	{
		Vector3 toTrigger = m_player->GetPosition();
		toTrigger -= {0.0f, 0.0f, 1400.0f};

		if (toTrigger.LengthSq() < 300.0f * 300.0f && m_phase == 0)
		{
			//ダッシュのチュートリアルを表示
			NewGO<DashTutorial>(0, "dashtutorial");

			m_phase = 1;
			m_phaseWaitTime = 0;
		}

		if(m_phase == 1 && m_phaseWaitTime == PHASE_WAIT_TIME)
		{
			MiniEnemy* enemy = NewGO<MiniEnemy>(0, "enemy");
			enemy->SetPosition(MINION_POSITION_PHASE1);

			m_gameSceneBGM->SoundChange();

			SoundOneShotPlay(WARP_SOUND_PATH);

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

			m_gameSceneBGM->SoundChange();

			SoundOneShotPlay(WARP_SOUND_PATH);

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
			}
			
			SoundOneShotPlay(WARP_SOUND_PATH);

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
			}

			SoundOneShotPlay(WARP_SOUND_PATH);

			m_phase = 4;
		}

		m_phaseWaitTime++;

		BossTank* bosstank = FindGO<BossTank>("bosstank");
		if (bosstank == nullptr && m_phase == 8)
		{
			m_phase = 9;
			m_phaseWaitTime = 0;
		}

		if (m_phase == 9 && m_phaseWaitTime == 280)
		{
			Fade* fade = NewGO<Fade>(0);
			fade->SetFadeInRate(0.01f);
			fade->SetWaitFrame(10);
			fade->SetFadeOutRate(0.01f);
		}

		if (m_phase == 9 && m_phaseWaitTime == 380)
		{
			NewGO<TitleScene>(0, "title");

			DeleteGO(this);
		}
	}

	void GameScene::NotifyGameOver()
	{
		if (m_isGameOver)
		{
			return;
		}

		m_phase = 9;
		m_phaseWaitTime = 100;

		m_isGameOver = true;
	}

	void GameScene::DeleteProducts()
	{
		ProductsDeleter productsDeleter;
		productsDeleter.DeleteProducts();
	}
}