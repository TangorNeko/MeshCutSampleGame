#include "stdafx.h"
#include "EventRollingTask.h"
#include "BossTank.h"
#include "Player.h"

namespace
{
	const float ROLLING_DEG = 15.0f;									//1フレームの回転角度
	const int ROLLING_TIME_START = 40;									//回転攻撃の開始フレーム
	const int ROLLING_TIME_END = 63;									//回転攻撃の終了フレーム
	const float ROLLING_KNOCKDOWN_POWER = 60.0f;						//吹き飛ばし力
	const wchar_t* ROLLING_SOUND_PATH = L"Assets/sound/RollingSE.wav";	//回転の効果音パス
}

namespace Game
{
	void EventRollingTask::SubmitTo(BossTank* bossTank, int taskNum)
	{
		EnemyTask EventRollingTask;

		EventRollingTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				if (taskTime == ROLLING_TIME_START)
				{
					//回転サウンドを再生
					SoundOneShotPlay(ROLLING_SOUND_PATH);
				}

				if (taskTime >= ROLLING_TIME_START)
				{
					//毎フレーム少しずつ回転させていく
					bossTank->SetTurretDeg(bossTank->GetTurretDeg() + ROLLING_DEG);
				}

				//規定フレームになったら終了
				if (taskTime == ROLLING_TIME_END)
				{
					//プレイヤーへの向きを取得
					Player* player = FindGO<Player>("player");
					Vector3 distance = player->GetPosition() - bossTank->GetPosition();

					distance.y = 0.0f;
					distance.Normalize();

					//プレイヤーを大きく吹き飛ばす
					player->KnockDown(distance * ROLLING_KNOCKDOWN_POWER);

					return true;
				}

				return false;
			}
		);

		//ボスにこのタスクをセットする
		bossTank->SetTask(taskNum, EventRollingTask);
	}
}