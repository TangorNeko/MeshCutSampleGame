#include "stdafx.h"
#include "Phase5.h"
#include "Fade.h"
#include "GameScene.h"


namespace
{
	const int FADE_START_TIME = 280;		//フェードが開始されるフレーム
	const int GAMESCENE_DELETE_TIME = 380;	//ゲームシーンを削除して再生成するフレーム
	const float FADE_RATE = 0.01f;			//フェード率
	const int FADE_WAIT_FRAME = 10;			//フェードが完了した後待機するフレーム数
}

namespace Game
{
	bool Phase5::PhaseUpdate()
	{
		//フェードが開始されるフレームだったら
		if (GetPhaseWaitFrame() == FADE_START_TIME)
		{
			//フェードを開始する
			StartFade();
		}

		//ゲームシーンを削除して再生成するフレームだったら
		if (GetPhaseWaitFrame() == GAMESCENE_DELETE_TIME)
		{
			//現在のゲームシーンを削除
			DeleteGO(FindGO<GameScene>("gamescene"));

			//新しいゲームシーンを生成
			NewGO<GameScene>(Priority::High, "gamescene");
		}

		return false;
	}

	void Phase5::StartFade()
	{
		//フェードを開始させる
		Fade* fade = NewGO<Fade>(Priority::High);
		fade->SetFadeInRate(FADE_RATE);
		fade->SetWaitFrame(FADE_WAIT_FRAME);
		fade->SetFadeOutRate(FADE_RATE);
	}
}
