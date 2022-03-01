#include "stdafx.h"
#include "WaitTask.h"
#include "BossTank.h"

namespace
{
	const int WAIT_TIME = 150;	//�ҋ@�t���[����
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
					//�ҋ@���ԕ��ҋ@������^�X�N�͏I��
					return true;
				}
				else
				{
					return false;
				}
			}
		);

		//�{�X�ɂ��̃^�X�N���Z�b�g����
		bossTank->SetTask(taskNum, WaitTask);
	}
}