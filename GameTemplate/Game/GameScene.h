#pragma once

namespace Game
{
	class BackGround;
	class Player;
	class EnemyMissile;
	class BossTank;

	class GameScene : public IGameObject
	{
		~GameScene();

		bool Start() override;

		void Update() override;

	private:
		BackGround* m_backGround = nullptr;						//ステージクラス
		Player* m_player = nullptr;								//プレイヤークラス
		BossTank* m_bossTank = nullptr;							//戦車のボスクラス
	};
}

