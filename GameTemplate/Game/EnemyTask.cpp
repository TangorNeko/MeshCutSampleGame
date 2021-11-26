#include "stdafx.h"
#include "EnemyTask.h"

namespace Game
{
	void EnemyTask::TaskStart()
	{
		m_taskStartFunc();
		m_taskState = enUpdate;
	}

	void EnemyTask::TaskUpdate()
	{
		bool isEnd = m_taskUpdateFunc(m_taskTime);

		m_taskTime++;

		if (isEnd == true)
		{
			m_taskState = enEnd;
		}
	}

	void EnemyTask::TaskEnd()
	{
		m_taskEndFunc();
		m_taskState = enExit;
	}

	bool EnemyTask::Execute()
	{
		switch(m_taskState)
		{
		case enStart:
			TaskStart();
			break;
		case enUpdate:
			TaskUpdate();
			break;
		case enEnd:
			TaskEnd();
			break;
		case enExit:
			return true;
		}

		return false;
	}

	void EnemyTask::Terminate()
	{
		if (m_taskState == enUpdate || m_taskState == enEnd)
		{
			TaskEnd();
		}

		m_taskState = enExit;
	}
}