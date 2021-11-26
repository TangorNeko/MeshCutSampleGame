#include "stdafx.h"
#include "BossTankTasks.h"
#include "BossTank.h"
#include "EnemyMissile.h"

namespace Game
{
	void BossTankTasks::SubmitTo(BossTank* bossTank)
	{
		SubmitMissileTask(bossTank);
		SubmitWaitTask(bossTank);
	}

	void BossTankTasks::SubmitMissileTask(BossTank* bossTank)
	{
		EnemyTask MissileTask;

		MissileTask.SetStartFunc([bossTank]()
			{
				EnemyMissile* missile = NewGO<EnemyMissile>(0, "missile");
				Vector3 pos = bossTank->GetPosition();
				pos.y += 200.0f;
				missile->SetPosition(pos);
			}
		);

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

		MissileTask.SetEndFunc([bossTank]()
			{
				bossTank->SetTurretDeg(bossTank->GetTurretDeg() + 30.0f);
			}
		);

		bossTank->SetTask(enMissile, MissileTask);
	}

	void BossTankTasks::SubmitRollingTask(BossTank* bossTank)
	{
		EnemyTask RollingTask;


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
				if (taskTime == 360)
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