#include "stdafx.h"
#include "GameScene.h"
#include "BackGround.h"
#include "Player.h"
#include "ProductsDeleter.h"
#include "GameSceneBGM.h"

namespace
{
	const Vector3 PLAYER_START_POSITION = { -1.0f,30.0f,3700.0f };	//プレイヤーのスタート座標
	const int GAMEOVER_PHASE = 5;									//ゲームオーバーのフェーズ番号
}

namespace Game
{
	GameScene::~GameScene()
	{
		//各種オブジェクトを削除
		DeleteGO(m_backGround);
		DeleteGO(m_player);
		DeleteGO(m_gameSceneBGM);
		DeleteProducts();
	}

	bool GameScene::Start()
	{
		//ゲームの背景を作成
		m_backGround = NewGO<BackGround>(Priority::High,"background");

		//プレイヤーを作成
		m_player = NewGO<Player>(Priority::High, "player");
		m_player->SetPosition(PLAYER_START_POSITION);
		m_player->EventCutOn();

		//BGMを作成
		m_gameSceneBGM = NewGO<GameSceneBGM>(Priority::High, "gamescenebgm");

		//ゲームフェーズの初期化
		m_gamephase.Init();
		return true;
	}

	void GameScene::Update()
	{
		//フェーズのアップデート処理
		m_gamephase.Update();
	}

	void GameScene::NotifyGameOver()
	{
		//ゲームフェーズをゲーム終了のものに
		m_gamephase.SkipTo(GAMEOVER_PHASE);
	}

	void GameScene::DeleteProducts()
	{
		//ゲームシーンによって生成されたオブジェクトを削除する
		ProductsDeleter productsDeleter;
		productsDeleter.DeleteProducts();
	}

	void GameScene::GameBGMChange()
	{
		//ゲームBGMを一段階進行させる
		m_gameSceneBGM->SoundChange();
	}
}