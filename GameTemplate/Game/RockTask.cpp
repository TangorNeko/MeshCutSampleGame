#include "stdafx.h"
#include "RockTask.h"
#include "BossTank.h"
#include "EnemyRock.h"
#include "Player.h"

namespace
{
	const float ROCK_SHOT_HEIGHT = 200.0f;								//岩を発射する高さ
	const float ROCK_SHOT_XOFFSET = 100.0f;								//岩を発射するX座標のオフセット
	const int ROCK_TIME_FIRST = 40;										//1発目の岩を発射するフレーム
	const int ROCK_TIME_SECOND = 45;									//2発目の岩を発射するフレーム
	const int ROCK_TIME_THIRD = 48;										//3発目の岩を発射するフレーム
	const char16_t* EFFECT_WARNING_PATH = u"Assets/effect/Warning.efk";	//警告エフェクトパス
	const Vector3 EFFECT_WARNING_HEIGHT = { 0.0f,250.0f,0.0f };			//警告エフェクトの高さ
}

namespace Game
{
	void RockTask::SubmitTo(BossTank* bossTank, int taskNum)
	{
		EnemyTask RockTask;

		RockTask.SetStartFunc([bossTank]()
			{
				//警告エフェクトを再生
				Effect* warningEffect = NewGO<Effect>(Priority::High);
				warningEffect->Init(EFFECT_WARNING_PATH);
				warningEffect->SetPosition(bossTank->GetPosition() + EFFECT_WARNING_HEIGHT);
				warningEffect->Play();
			}
		);

		//岩を作成する関数
		auto ShotRock = [bossTank](const Vector3& pos) {
			EnemyRock* rock = NewGO<EnemyRock>(Priority::High, "rock");
			Player* player = FindGO<Player>("player");
			Vector3 distance = player->GetPosition() - bossTank->GetPosition();
			distance.Normalize();
			rock->SetPosition(pos);
			rock->SetDirection(distance);
		};

		RockTask.SetUpdateFunc([bossTank, ShotRock](int taskTime)
			{
				//規定フレームになると岩を発射
				if (taskTime == ROCK_TIME_FIRST)
				{
					Vector3 pos = bossTank->GetPosition();
					pos.y += ROCK_SHOT_HEIGHT;
					ShotRock(pos);
				}

				if (taskTime == ROCK_TIME_SECOND)
				{
					Vector3 pos = bossTank->GetPosition();
					pos.y += ROCK_SHOT_HEIGHT;
					pos.x += ROCK_SHOT_XOFFSET;
					ShotRock(pos);
				}

				if (taskTime == ROCK_TIME_THIRD)
				{
					Vector3 pos = bossTank->GetPosition();
					pos.y += ROCK_SHOT_HEIGHT;
					pos.x -= ROCK_SHOT_XOFFSET;
					ShotRock(pos);

					//3つ発射したらタスクは終了
					return true;
				}


				return false;
			}
		);

		//ボスにこのタスクをセットする
		bossTank->SetTask(taskNum, RockTask);
	}
}