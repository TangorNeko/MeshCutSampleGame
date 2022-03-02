#include "stdafx.h"
#include "Phase4.h"
#include "BossTank.h"
#include "GameScene.h"

namespace
{
	int PHASE_START_FRAME = 180;									//フェーズが開始されるまでのフレーム数
	const Vector3 BOSS_POSITION = { 0.0f,0.0f,-1300.0f };			//ボスを召喚する座標
	const wchar_t* WARP_SOUND_PATH = L"Assets/sound/WarpSE.wav";	//ボス出現効果音パス
}

namespace Game
{
	void Phase4::Init()
	{
		//フェーズ開始までのフレームをセット
		SetPhaseStartFrame(PHASE_START_FRAME);
	}

	void Phase4::PhaseStart()
	{
		//ボスを召喚
		BossTank* bossTank = NewGO<BossTank>(Priority::High, "bosstank");
		bossTank->SetPosition(BOSS_POSITION);

		//召喚の効果音を再生
		SoundOneShotPlay(WARP_SOUND_PATH);

		//BGMを変更する
		GameScene* gameScene = FindGO<GameScene>("gamescene");
		gameScene->GameBGMChange();
	}

	bool Phase4::PhaseUpdate()
	{
		//ボスがいるかを検索
		BossTank* bosstank = FindGO<BossTank>("bosstank");

		//ボスがいなかったら
		if (bosstank == nullptr)
		{
			//フェーズ終了
			return true;
		}

		return false;
	}
}
