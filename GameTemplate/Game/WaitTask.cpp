#include "stdafx.h"
#include "WaitTask.h"
#include "BossTank.h"

namespace
{
	const int WAIT_TIME = 150;	//待機フレーム数
}

namespace Game
{
	void WaitTask::SubmitTo(BossTank* bossTank, int taskNum)
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
		bossTank->SetTask(taskNum, WaitTask);
	}
}