#include "stdafx.h"
#include "BossTankTasks.h"
#include "BossTank.h"
#include "MissileTask.h"
#include "RollingTask.h"
#include "SummonTask.h"
#include "ChargeTask.h"
#include "RockTask.h"
#include "WaitTask.h"
#include "StepTask.h"
#include "EventRollingTask.h"

namespace Game
{
	void BossTankTasks::SubmitTo(BossTank* bossTank)
	{
		//ボスに各タスクを設定する
		SubmitMissileTask(bossTank);
		SubmitWaitTask(bossTank);
		SubmitRollingTask(bossTank);
		SubmitSummonTask(bossTank);
		SubmitRockTask(bossTank);
		SubmitChargeTask(bossTank);
		SubmitStepTask(bossTank);
		SubmitEventRollingTask(bossTank);
	}

	void BossTankTasks::SubmitMissileTask(BossTank* bossTank)
	{
		MissileTask missileTask;
		missileTask.SubmitTo(bossTank, enMissile);
	}

	void BossTankTasks::SubmitRollingTask(BossTank* bossTank)
	{
		RollingTask rollingTask;
		rollingTask.SubmitTo(bossTank,enRolling);
	}

	void BossTankTasks::SubmitSummonTask(BossTank* bossTank)
	{
		SummonTask summonTask;
		summonTask.SubmitTo(bossTank, enSummon);
	}

	void BossTankTasks::SubmitChargeTask(BossTank* bossTank)
	{
		ChargeTask chargeTask;
		chargeTask.SubmitTo(bossTank, enCharge);
	}

	void BossTankTasks::SubmitRockTask(BossTank* bossTank)
	{
		RockTask rockTask;
		rockTask.SubmitTo(bossTank, enRock);
	}

	void BossTankTasks::SubmitWaitTask(BossTank* bossTank)
	{
		WaitTask waitTask;
		waitTask.SubmitTo(bossTank, enWait);
	}

	void BossTankTasks::SubmitStepTask(BossTank* bossTank)
	{
		StepTask stepTask;
		stepTask.SubmitTo(bossTank, enStep);
	}

	void BossTankTasks::SubmitEventRollingTask(BossTank* bossTank)
	{
		EventRollingTask eventRollingTask;
		eventRollingTask.SubmitTo(bossTank, enEventRolling);
	}
}