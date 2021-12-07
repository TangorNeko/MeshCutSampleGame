#include "stdafx.h"
#include "BossTankTasks.h"
#include "BossTank.h"
#include "EnemyMissile.h"
#include "Player.h"
#include "MiniEnemy.h"
#include "EnemyRock.h"

namespace
{
	const Vector3 MINION_POSITIONS[4] = { { 850.0f,0.0f,-1400.0f }, { -850.0f,0.0f,-1400.0f } ,{ 1300.0f,0.0f,-1000.0f } ,{ -1300.0f,0.0f,-1000.0f } };
	const int MISSILE_TIME_FIRST = 30;
	const int MISSILE_TIME_SECOND = 60;
	const int MISSILE_TIME_THIRD = 90;
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

		MissileTask.SetUpdateFunc([bossTank](int taskTime)
			{
				if (taskTime == MISSILE_TIME_FIRST)
				{
					EnemyMissile* missile = NewGO<EnemyMissile>(0, "missile");
					Vector3 pos = bossTank->GetPosition();
					pos.y += 200.0f;
					missile->SetPosition(pos);
				}

				if (taskTime == MISSILE_TIME_SECOND)
				{
					EnemyMissile* missile = NewGO<EnemyMissile>(0, "missile");
					Vector3 pos = bossTank->GetPosition();
					pos.y += 200.0f;
					missile->SetPosition(pos);
				}

				if (taskTime == MISSILE_TIME_THIRD)
				{
					EnemyMissile* missile = NewGO<EnemyMissile>(0, "missile");
					Vector3 pos = bossTank->GetPosition();
					pos.y += 200.0f;
					missile->SetPosition(pos);
					return true;
				}

				return false;
			}
		);

		bossTank->SetTask(enMissile, MissileTask);
	}

	void BossTankTasks::SubmitRollingTask(BossTank* bossTank)
	{
		EnemyTask RollingTask;

		RollingTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				bossTank->SetTurretDeg(bossTank->GetTurretDeg() + 15.0f);

				if (taskTime == 23)
				{
					//TODO:距離ではなくトリガーで判定したい所
					Player* player = FindGO<Player>("player");
					Vector3 distance = player->GetPosition() - bossTank->GetPosition();
					if (distance.LengthSq() < 700 * 700 && player->isGuard() == false)
					{
						distance.Normalize();
						player->KnockDown(distance * 60);
					}
					return true;
				}

				return false;
			}
		);

		bossTank->SetTask(enRolling, RollingTask);
	}

	void BossTankTasks::SubmitSummonTask(BossTank* bossTank)
	{
		EnemyTask SummonTask;

		SummonTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				if (taskTime == 30)
				{
					MiniEnemy* enemy[4];
					for (int i = 0; i < 4; i++)
					{
						enemy[i] = NewGO<MiniEnemy>(0, "enemy");
						enemy[i]->SetPosition(MINION_POSITIONS[i]);
					}

					return true;
				}

				return false;
			}
		);

		bossTank->SetTask(enSummon, SummonTask);
	}

	void BossTankTasks::SubmitChargeTask(BossTank* bossTank)
	{
		EnemyTask ChargeTask;

		ChargeTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				bossTank->SetBaseDeg(bossTank->GetBaseDeg() + 15.0f);

				if (taskTime == 23)
				{
					//TODO:距離ではなくトリガーで判定したい所
					Player* player = FindGO<Player>("player");
					Vector3 distance = player->GetPosition() - bossTank->GetPosition();
					if (distance.LengthSq() < 700 * 700 && player->isGuard() == false)
					{
						distance.Normalize();
						player->KnockDown(distance * 60);
					}
					return true;
				}

				return false;
			}
		);

		bossTank->SetTask(enCharge, ChargeTask);
	}

	void BossTankTasks::SubmitRockTask(BossTank* bossTank)
	{
		EnemyTask RockTask;

		auto createRock = [bossTank](const Vector3& pos) {
			EnemyRock* rock = NewGO<EnemyRock>(0, "rock");
			Player* player = FindGO<Player>("player");
			Vector3 distance = player->GetPosition() - bossTank->GetPosition();
			distance.Normalize();
			rock->SetPosition(pos);
			rock->SetDirection(distance);
		};

		RockTask.SetUpdateFunc([bossTank,createRock](int taskTime)
			{
				if (taskTime == 30)
				{
					Vector3 pos = bossTank->GetPosition();
					pos.y += 200.0f;
					createRock(pos);
				}

				if (taskTime == 35)
				{
					Vector3 pos = bossTank->GetPosition();
					pos.y += 200.0f;
					pos.x += 100.0f;
					createRock(pos);
				}

				if (taskTime == 38)
				{
					Vector3 pos = bossTank->GetPosition();
					pos.y += 200.0f;
					pos.x -= 100.0f;
					createRock(pos);

					return true;
				}


				return false;
			}
		);

		bossTank->SetTask(enRock, RockTask);
	}

	void BossTankTasks::SubmitWaitTask(BossTank* bossTank)
	{
		EnemyTask WaitTask;

		WaitTask.SetUpdateFunc([](int taskTime)
			{
				if (taskTime == 180)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		);

		bossTank->SetTask(enWait, WaitTask);
	}
}