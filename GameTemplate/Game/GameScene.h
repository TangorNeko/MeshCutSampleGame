#pragma once

namespace Game
{
	class BackGround;
	class Player;
	class EnemyMissile;
	class BossTank;

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
	private:
		BackGround* m_backGround = nullptr;						//ステージクラス
		Player* m_player = nullptr;								//プレイヤークラス
		BossTank* m_bossTank = nullptr;							//戦車のボスクラス

		int m_phase = 0;
		int m_phaseWaitTime = 0;
	};
}

