#include "stdafx.h"
#include "Phase1.h"
#include "MiniEnemy.h"
#include "GameScene.h"

namespace
{
	int PHASE_START_FRAME = 180;									//フェーズが開始されるまでのフレーム数
	const Vector3 MINION_POSITION_PHASE1 = { 0.0f,0.0f,-500.0f };	//雑魚敵を召喚する座標
	const wchar_t* WARP_SOUND_PATH = L"Assets/sound/WarpSE.wav";	//雑魚敵出現効果音パス
}

namespace Game
{
	void Phase1::Init()
	{
		//フェーズ開始までのフレームをセット
		SetPhaseStartFrame(PHASE_START_FRAME);
	}

	void Phase1::PhaseStart()
	{
		//雑魚敵を召喚
		MiniEnemy* enemy = NewGO<MiniEnemy>(Priority::High, "enemy");
		enemy->SetPosition(MINION_POSITION_PHASE1);

		//召喚の効果音を再生
		SoundOneShotPlay(WARP_SOUND_PATH);
		
		//BGMを変更する
		GameScene* gameScene = FindGO<GameScene>("gamescene");
		gameScene->GameBGMChange();
	}

	bool Phase1::PhaseUpdate()
	{
		//雑魚敵がいるかを検索
		MiniEnemy* enemy = FindGO<MiniEnemy>("enemy");

		//雑魚敵がいなかったら
		if (enemy == nullptr)
		{
			//フェーズ終了
			return true;
		}

		return false;
	}
}