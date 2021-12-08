#include "stdafx.h"
#include "BossTankTasks.h"
#include "BossTank.h"
#include "EnemyMissile.h"
#include "Player.h"
#include "MiniEnemy.h"
#include "EnemyRock.h"

namespace
{
	const int MISSILE_TIME_FIRST = 30;
	const int MISSILE_TIME_SECOND = 60;
	const int MISSILE_TIME_THIRD = 90;
	const float MISSILE_SHOT_HEIGHT = 200.0f;
	const float ROLLING_DEG = 15.0f;
	const int ROLLING_TIME_END = 23;
	const float ROLLING_RANGE = 700.0f;
	const float ROLLING_KNOCKDOWN_POWER = 60.0f;
	const int SUMMON_TIME = 30;
	const int SUMMON_NUM = 4;
	const Vector3 MINION_POSITIONS[SUMMON_NUM] = { { 850.0f,0.0f,-1400.0f }, { -850.0f,0.0f,-1400.0f } ,{ 1300.0f,0.0f,-1000.0f } ,{ -1300.0f,0.0f,-1000.0f } };
	const float ROCK_SHOT_HEIGHT = 200.0f;
	const float ROCK_SHOT_XOFFSET = 100.0f;
	const int ROCK_TIME_FIRST = 30;
	const int ROCK_TIME_SECOND = 35;
	const int ROCK_TIME_THIRD = 38;
	const int WAIT_TIME = 180;
}

namespace Game
{
	void BossTankTasks::SubmitTo(BossTank* bossTank)
	{
		SubmitMissileTask(bossTank);
		SubmitWaitTask(bossTank);
		SubmitRollingTask(bossTank);
		SubmitSummonTask(bossTank);
		SubmitRockTask(bossTank);
		SubmitChargeTask(bossTank);
	}

	void BossTankTasks::SubmitMissileTask(BossTank* bossTank)
	{
		EnemyTask MissileTask;

		//ミサイルを発射する処理
		auto ShotMissile = [bossTank]()
		{
			EnemyMissile* missile = NewGO<EnemyMissile>(0, "missile");
			Vector3 pos = bossTank->GetPosition();
			pos.y += MISSILE_SHOT_HEIGHT;
			missile->SetPosition(pos);
		};

		MissileTask.SetUpdateFunc([bossTank,ShotMissile](int taskTime)
			{
				//規定フレームになるとミサイルを発射する
				if (taskTime == MISSILE_TIME_FIRST)
				{
					ShotMissile();
				}

				if (taskTime == MISSILE_TIME_SECOND)
				{
					ShotMissile();
				}

				if (taskTime == MISSILE_TIME_THIRD)
				{
					ShotMissile();

					//三発目を撃ったらタスクは終了
					return true;
				}

				return false;
			}
		);

		//ボスにこのタスクをセットする
		bossTank->SetTask(enMissile, MissileTask);
	}

	void BossTankTasks::SubmitRollingTask(BossTank* bossTank)
	{
		EnemyTask RollingTask;

		RollingTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				//毎フレーム少しずつ回転させていく
				bossTank->SetTurretDeg(bossTank->GetTurretDeg() + ROLLING_DEG);

				//規定フレームになったら終了
				if (taskTime == ROLLING_TIME_END)
				{
					return true;
				}

				return false;
			}
		);

		//タスクの終了時にダメージ判定をする
		RollingTask.SetEndFunc([bossTank]()
			{
				//TODO:距離ではなくトリガーで判定したい所
				Player* player = FindGO<Player>("player");
				Vector3 distance = player->GetPosition() - bossTank->GetPosition();

				//プレイヤーとの距離が近かったら
				if (distance.LengthSq() < ROLLING_RANGE * ROLLING_RANGE && player->isGuard() == false)
				{
					distance.Normalize();

					//プレイヤーを大きく吹き飛ばす
					player->KnockDown(distance * 60);
				}
			}
		);

		//ボスにこのタスクをセットする
		bossTank->SetTask(enRolling, RollingTask);
	}

	void BossTankTasks::SubmitSummonTask(BossTank* bossTank)
	{
		EnemyTask SummonTask;

		SummonTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				if (taskTime == SUMMON_TIME)
				{
					//雑魚敵を召喚
					MiniEnemy* enemy[SUMMON_NUM];
					for (int i = 0; i < SUMMON_NUM; i++)
					{
						enemy[i] = NewGO<MiniEnemy>(0, "enemy");
						enemy[i]->SetPosition(MINION_POSITIONS[i]);
					}

					//召喚したらタスクは終わり
					return true;
				}

				return false;
			}
		);

		//ボスにこのタスクをセットする
		bossTank->SetTask(enSummon, SummonTask);
	}

	void BossTankTasks::SubmitChargeTask(BossTank* bossTank)
	{
		EnemyTask ChargeTask;

		ChargeTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				//毎フレーム少しずつ回転させていく
				bossTank->SetBaseDeg(bossTank->GetBaseDeg() + ROLLING_DEG);

				//規定フレームになったら終了
				if (taskTime == ROLLING_TIME_END)
				{
					return true;
				}

				return false;
			}
		);

		//タスクの終了時にダメージ判定をする
		ChargeTask.SetEndFunc([bossTank]()
			{
				//TODO:距離ではなくトリガーで判定したい所
				Player* player = FindGO<Player>("player");
				Vector3 distance = player->GetPosition() - bossTank->GetPosition();

				//プレイヤーとの距離が近かったら
				if (distance.LengthSq() < ROLLING_RANGE * ROLLING_RANGE && player->isGuard() == false)
				{
					distance.Normalize();

					//プレイヤーを大きく吹き飛ばす
					player->KnockDown(distance * ROLLING_KNOCKDOWN_POWER);
				}
			}
		);

		//ボスにこのタスクをセットする
		bossTank->SetTask(enCharge, ChargeTask);
	}

	void BossTankTasks::SubmitRockTask(BossTank* bossTank)
	{
		EnemyTask RockTask;

		//岩を作成する関数
		auto ShotRock = [bossTank](const Vector3& pos) {
			EnemyRock* rock = NewGO<EnemyRock>(0, "rock");
			Player* player = FindGO<Player>("player");
			Vector3 distance = player->GetPosition() - bossTank->GetPosition();
			distance.Normalize();
			rock->SetPosition(pos);
			rock->SetDirection(distance);
		};

		RockTask.SetUpdateFunc([bossTank,ShotRock](int taskTime)
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
		bossTank->SetTask(enRock, RockTask);
	}

	void BossTankTasks::SubmitWaitTask(BossTank* bossTank)
	{
		EnemyTask WaitTask;

		WaitTask.SetUpdateFunc([](int taskTime)
			{
				if (taskTime == WAIT_TIME)
				{
					//待機時間分待機したらタスクは終了
					return true;
				}
				else
				{
					return false;
				}
			}
		);

		//ボスにこのタスクをセットする
		bossTank->SetTask(enWait, WaitTask);
	}
}