#include "stdafx.h"
#include "BossTankTasks.h"
#include "BossTank.h"
#include "EnemyMissile.h"
#include "Player.h"

namespace Game
{
	void BossTankTasks::SubmitTo(BossTank* bossTank)
	{
		SubmitMissileTask(bossTank);
		SubmitWaitTask(bossTank);
		SubmitRollingTask(bossTank);
	}

	void BossTankTasks::SubmitMissileTask(BossTank* bossTank)
	{
		EnemyTask MissileTask;

		MissileTask.SetUpdateFunc([bossTank](int taskTime)
			{
				if (taskTime == 20)
				{
					EnemyMissile* missile = NewGO<EnemyMissile>(0, "missile");
					Vector3 pos = bossTank->GetPosition();
					pos.y += 200.0f;
					missile->SetPosition(pos);
				}

				if (taskTime == 40)
				{
					EnemyMissile* missile = NewGO<EnemyMissile>(0, "missile");
					Vector3 pos = bossTank->GetPosition();
					pos.y += 200.0f;
					missile->SetPosition(pos);
				}

				if (taskTime == 60)
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
					//TODO:‹——£‚Å‚Í‚È‚­ƒgƒŠƒK[‚Å”»’è‚µ‚½‚¢Š
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
	}

	void BossTankTasks::SubmitChargeTask(BossTank* bossTank)
	{
		EnemyTask ChargeTask;
	}

	void BossTankTasks::SubmitRockTask(BossTank* bossTank)
	{
		EnemyTask RockTask;
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