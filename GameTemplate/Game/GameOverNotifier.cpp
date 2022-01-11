#include "stdafx.h"
#include "GameOverNotifier.h"
#include "GameScene.h"

namespace Game
{
	void GameOverNotifier::NotifyGameOver()
	{
		GameScene* gamescene = FindGO<GameScene>("gamescene");
		gamescene->NotifyGameOver();
	}
}
