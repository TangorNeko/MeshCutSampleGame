#include "stdafx.h"
#include "MissileTask.h"
#include "BossTank.h"
#include "EnemyMissile.h"

namespace
{
	const int MISSILE_TIME_FIRST = 40;											//1発目のミサイル発射フレーム
	const int MISSILE_TIME_SECOND = 70;											//2発目のミサイル発射フレーム
	const int MISSILE_TIME_THIRD = 100;											//3発目のミサイル発射フレーム
	const wchar_t* MISSILE_SOUND_PATH = L"Assets/sound/MissileLaunchSE.wav";	//ミサイル発射の効果音パス
	const char16_t* EFFECT_WARNING_PATH = u"Assets/effect/Warning.efk";			//警告エフェクトパス
	const Vector3 EFFECT_WARNING_HEIGHT = { 0.0f,250.0f,0.0f };					//警告エフェクトの高さ
}

namespace Game
{
	void MissileTask::SubmitTo(BossTank* bossTank,int taskNum)
	{
		EnemyTask MissileTask;

		//ミサイルを発射する処理
		auto ShotMissile = [bossTank](const Vector3& position)
		{
			//ミサイルを作成
			EnemyMissile* missile = NewGO<EnemyMissile>(Priority::High, "missile");
			missile->SetPosition(position);

			//発射サウンドを再生
			SoundOneShotPlay(MISSILE_SOUND_PATH);
		};

		MissileTask.SetStartFunc([bossTank]()
			{
				//警告エフェクトを再生
				Effect* warningEffect = NewGO<Effect>(Priority::High);
				warningEffect->Init(EFFECT_WARNING_PATH);
				warningEffect->SetPosition(bossTank->GetPosition() + EFFECT_WARNING_HEIGHT);
				warningEffect->Play();
			}
		);

		MissileTask.SetUpdateFunc([bossTank, ShotMissile](int taskTime)
			{
				//規定フレームになるとミサイルを発射する
				if (taskTime == MISSILE_TIME_FIRST)
				{
					ShotMissile(bossTank->GetRightCannonPosition());
				}

				if (taskTime == MISSILE_TIME_SECOND)
				{
					ShotMissile(bossTank->GetLeftCannonPosition());
				}

				if (taskTime == MISSILE_TIME_THIRD)
				{
					ShotMissile(bossTank->GetRightCannonPosition());

					//三発目を撃ったらタスクは終了
					return true;
				}

				return false;
			}
		);

		//ボスにこのタスクをセットする
		bossTank->SetTask(taskNum, MissileTask);
	}
}