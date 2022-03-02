#pragma once
#include "GamePhase.h"

namespace Game
{
	class BackGround;
	class Player;
	class GameSceneBGM;

	class GameScene : public IGameObject
	{
	public:
		~GameScene();

		bool Start() override;

		void Update() override;

		/**
		 * @brief ゲームシーンで生成された小物を削除する
		*/
		void DeleteProducts();

		/**
		 * @brief ゲームオーバーを通知する
		*/
		void NotifyGameOver();

		/**
		 * @brief ゲームのBGMを1段階変更する
		*/
		void GameBGMChange();
	private:
		BackGround* m_backGround = nullptr;						//ステージクラス
		Player* m_player = nullptr;								//プレイヤークラス
		GameSceneBGM* m_gameSceneBGM = nullptr;					//BGM再生クラス
		GamePhase m_gamephase;									//ゲームフェーズ
	};
}

