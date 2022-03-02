#include "stdafx.h"
#include "Phase2.h"
#include "MiniEnemy.h"

namespace
{
	int PHASE_START_FRAME = 180;								//フェーズが開始されるまでのフレーム数
	const int SUMMON_NUM_PHASE2 = 2;							//雑魚敵の召喚数
	const Vector3 MINION_POSITIONS_PHASE2[SUMMON_NUM_PHASE2] =	//雑魚敵を召喚する座標
	{
		{-400.0f,0.0f,-800.0f},
		{400.0f,0.0f,-800.0f}
	};
	const wchar_t* WARP_SOUND_PATH = L"Assets/sound/WarpSE.wav";//雑魚敵出現効果音パス
}

namespace Game
{
	void Phase2::Init()
	{
		//フェーズ開始までのフレームをセット
		SetPhaseStartFrame(PHASE_START_FRAME);
	}

	void Phase2::PhaseStart()
	{
		//雑魚敵を複数召喚
		MiniEnemy* enemy[SUMMON_NUM_PHASE2];
		for (int i = 0; i < SUMMON_NUM_PHASE2; i++)
		{
			enemy[i] = NewGO<MiniEnemy>(Priority::High, "enemy");
			enemy[i]->SetPosition(MINION_POSITIONS_PHASE2[i]);
		}

		//召喚の効果音を再生
		SoundOneShotPlay(WARP_SOUND_PATH);
	}

	bool Phase2::PhaseUpdate()
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