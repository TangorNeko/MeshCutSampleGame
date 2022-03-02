#include "stdafx.h"
#include "Phase0.h"
#include "Player.h"
#include "DashTutorial.h"

namespace
{
	const Vector3 DASHTUTORIAL_TO_TRIGGER = { 0.0f, 0.0f, 1400.0f };	//走りのチュートリアルのトリガーへのベクトル
	const float DASHTUTORIAL_RANGE = 300.0f;							//走りのチュートリアルの表示が開始される範囲
}

namespace Game
{
	void Phase0::Init()
	{
		//プレイヤーを取得
		m_player = FindGO<Player>("player");
	}

	bool Phase0::PhaseUpdate()
	{
		//プレイヤーからトリガーまでの距離を計算
		Vector3 toTrigger = m_player->GetPosition();
		toTrigger -= DASHTUTORIAL_TO_TRIGGER;

		//プレイヤーがチュートリアル表示範囲内なら
		if (toTrigger.LengthSq() < DASHTUTORIAL_RANGE * DASHTUTORIAL_RANGE)
		{
			//ダッシュのチュートリアルを表示
			NewGO<DashTutorial>(Priority::High, "dashtutorial");

			//フェーズを終了
			return true;
		}

		return false;
	}
}