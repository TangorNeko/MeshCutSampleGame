#include "stdafx.h"
#include "GameScene.h"
#include "BackGround.h"
#include "Player.h"
#include "BossTank.h"
#include "MiniEnemy.h"
#include "Fade.h"
#include "ProductsDeleter.h"
#include "DashTutorial.h"
#include "GameSceneBGM.h"

namespace
{
	const Vector3 BOSS_POSITION = { 0.0f,0.0f,-1300.0f };
	const Vector3 PLAYER_START_POSITION = { -1.0f,30.0f,3700.0f };

	const Vector3 MINION_POSITION_PHASE1 = { 0.0f,0.0f,-500.0f };

	const int SUMMON_NUM_PHASE2 = 2;
	const Vector3 MINION_POSITIONS_PHASE2[SUMMON_NUM_PHASE2] = { {-400.0f,0.0f,-800.0f}, {400.0f,0.0f,-800.0f} };

	const int SUMMON_NUM_PHASE3 = 4;
	const Vector3 MINION_POSITIONS_PHASE3[SUMMON_NUM_PHASE3] = { { 850.0f,0.0f,-1400.0f }, { -850.0f,0.0f,-1400.0f } ,{ 1300.0f,0.0f,-1000.0f } ,{ -1300.0f,0.0f,-1000.0f } };

	const int PHASE_WAIT_TIME = 180;

	const wchar_t* WARP_SOUND_PATH = L"Assets/sound/WarpSE.wav";

	const Vector3 DASHTUTORIAL_TO_TRIGGER = { 0.0f, 0.0f, 1400.0f };
	const float DASHTUTORIAL_RANGE =  300.0f;

	const int GAMEOVER_TIME = 100;
	const int FADE_START_TIME = 280;
	const int GAMESCENE_DELETE_TIME = 380;
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
		m_backGround = NewGO<BackGround>(Priority::High,"background");
		m_player = NewGO<Player>(Priority::High, "player");
		m_player->SetPosition(PLAYER_START_POSITION);
		m_player->EventCutOn();

		m_gameSceneBGM = NewGO<GameSceneBGM>(Priority::High, "gamescenebgm");
		return true;
	}

	void GameScene::Update()
	{
		//フェーズに合わせて分岐
		//TODO:フェーズごとにクラスを分離する

		Vector3 toTrigger = m_player->GetPosition();
		toTrigger -= DASHTUTORIAL_TO_TRIGGER;

		if (toTrigger.LengthSq() < DASHTUTORIAL_RANGE * DASHTUTORIAL_RANGE && m_phase == 0)
		{
			//ダッシュのチュートリアルを表示
			NewGO<DashTutorial>(Priority::High, "dashtutorial");

			m_phase = 1;
			m_phaseWaitTime = 0;
		}

		if(m_phase == 1 && m_phaseWaitTime == PHASE_WAIT_TIME)
		{
			MiniEnemy* enemy = NewGO<MiniEnemy>(Priority::High, "enemy");
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
			m_bossTank = NewGO<BossTank>(Priority::High, "bosstank");
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
				enemy[i] = NewGO<MiniEnemy>(Priority::High, "enemy");
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
				enemy[i] = NewGO<MiniEnemy>(Priority::High, "enemy");
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

		if (m_phase == 9 && m_phaseWaitTime == FADE_START_TIME)
		{
			Fade* fade = NewGO<Fade>(Priority::High);
			fade->SetFadeInRate(0.01f);
			fade->SetWaitFrame(10);
			fade->SetFadeOutRate(0.01f);
		}

		if (m_phase == 9 && m_phaseWaitTime == GAMESCENE_DELETE_TIME)
		{
			NewGO<GameScene>(Priority::High, "gamescene");

			DeleteGO(this);
		}
	}

	void GameScene::NotifyGameOver()
	{
		//既にゲームオーバーなら何もしない
		if (m_isGameOver)
		{
			return;
		}

		m_phase = 9;
		m_phaseWaitTime = GAMEOVER_TIME;

		m_isGameOver = true;
	}

	void GameScene::DeleteProducts()
	{
		ProductsDeleter productsDeleter;
		productsDeleter.DeleteProducts();
	}
}