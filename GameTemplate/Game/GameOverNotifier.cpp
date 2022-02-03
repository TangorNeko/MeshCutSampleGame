#include "stdafx.h"
#include "GameOverNotifier.h"
#include "GameScene.h"

namespace Game
{
	void GameOverNotifier::NotifyGameOver()
	{
		//�Q�[���I�[�o�[��ʒm����
		GameScene* gamescene = FindGO<GameScene>("gamescene");
		gamescene->NotifyGameOver();
	}
}
