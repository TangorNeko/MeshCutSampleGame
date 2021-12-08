#include "stdafx.h"
#include "EnemyTask.h"

namespace Game
{
	void EnemyTask::TaskStart()
	{
		//スタート時に呼ばれる関数を実行する
		m_taskStartFunc();

		//タスクの状態をアップデート状態に遷移
		m_taskState = enUpdate;
	}

	void EnemyTask::TaskUpdate()
	{
		//アップデートで呼ばれる関数を実行し、終了したかどうかを受け取る
		bool isEnd = m_taskUpdateFunc(m_taskTime);

		//タスクの終了を受け取ったら、タスクの状態を終了状態に遷移
		if (isEnd == true)
		{
			m_taskState = enEnd;
		}

		//経過フレームを加算
		m_taskTime++;
	}

	void EnemyTask::TaskEnd()
	{
		//タスクの終了時に呼ばれる関数を実行する
		m_taskEndFunc();

		//タスクの状態をタスクから抜ける状態に遷移
		m_taskState = enExit;
	}

	bool EnemyTask::Execute()
	{
		//タスクの状態によって分岐
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
			//タスクから抜ける
			return true;
		}

		return false;
	}

	void EnemyTask::Terminate()
	{
		//タスクがアップデート中か終了時だったら(スタート時の処理が呼ばれていたら)
		if (m_taskState == enUpdate || m_taskState == enEnd)
		{
			//終了時の処理を呼ぶ
			TaskEnd();
		}

		//タスクの状態をタスクから抜ける状態に遷移
		m_taskState = enExit;
	}
}