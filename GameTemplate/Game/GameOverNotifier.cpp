#include "stdafx.h"
#include "GameOverNotifier.h"
#include "GameScene.h"

namespace Game
{
	void GameOverNotifier::NotifyGameOver()
	{
		//ゲームオーバーを通知する
		GameScene* gamescene = FindGO<GameScene>("gamescene");
		gamescene->NotifyGameOver();
	}
}
