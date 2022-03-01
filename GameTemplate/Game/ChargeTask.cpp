#include "stdafx.h"
#include "ChargeTask.h"
#include "BossTank.h"
#include "Player.h"

namespace
{
	const float ROLLING_DEG = 15.0f;									//1フレームの回転角度
	const int ROLLING_TIME_START = 40;									//回転攻撃の開始フレーム
	const int ROLLING_TIME_END = 63;									//回転攻撃の終了フレーム
	const float ROLLING_RANGE = 700.0f;									//吹き飛ばし範囲
	const float ROLLING_KNOCKDOWN_POWER = 60.0f;						//吹き飛ばし力
	const wchar_t* ROLLING_SOUND_PATH = L"Assets/sound/RollingSE.wav";	//回転の効果音パス
	const wchar_t* DAMAGE_SOUND_PATH = L"Assets/sound/DamageSE.wav";	//ダメージの効果音パス
	const char16_t* EFFECT_WARNING_PATH = u"Assets/effect/Warning.efk";	//警告エフェクトパス
	const Vector3 EFFECT_WARNING_HEIGHT = { 0.0f,250.0f,0.0f };			//警告エフェクトの高さ
}

namespace Game
{
	void ChargeTask::SubmitTo(BossTank* bossTank, int taskNum)
	{
		EnemyTask ChargeTask;

		ChargeTask.SetStartFunc([bossTank]()
			{
				//警告エフェクトを再生
				Effect* warningEffect = NewGO<Effect>(Priority::High);
				warningEffect->Init(EFFECT_WARNING_PATH);
				warningEffect->SetPosition(bossTank->GetPosition() + EFFECT_WARNING_HEIGHT);
				warningEffect->Play();
			}
		);

		ChargeTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				if (taskTime >= ROLLING_TIME_START)
				{
					//毎フレーム少しずつ回転させていく
					bossTank->SetBaseDeg(bossTank->GetBaseDeg() + ROLLING_DEG);
				}

				//規定フレームになったら終了
				if (taskTime == ROLLING_TIME_END)
				{
					Player* player = FindGO<Player>("player");
					Vector3 distance = player->GetPosition() - bossTank->GetPosition();

					//プレイヤーとの距離が近かったら
					if (distance.LengthSq() < ROLLING_RANGE * ROLLING_RANGE)
					{
						distance.Normalize();

						//プレイヤーを大きく吹き飛ばす
						player->KnockDown(distance * ROLLING_KNOCKDOWN_POWER);
						SoundOneShotPlay(DAMAGE_SOUND_PATH);
					}
					return true;
				}

				return false;
			}
		);

		//ボスにこのタスクをセットする
		bossTank->SetTask(taskNum, ChargeTask);
	}
}