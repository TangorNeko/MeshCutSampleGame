#include "stdafx.h"
#include "BossTankBehave.h"
#include "Player.h"
#include "BossTank.h"


namespace
{
	const float DISTANCE_RANGED_ATTACK = 1000.0f;
	const int MAX_HP = 400;
}

namespace Game
{
	void BossTankBehave::SubmitTo(BossTank* bossTank)
	{
		BossTankTasks bossTankTasks;
		bossTankTasks.SubmitTo(bossTank);
	}

	void BossTankBehave::Execute(BossTankStatus& bossTankStatus)
	{
		CheckEvent(bossTankStatus);

		//キューにタスクがない場合
		if (m_taskQueue.size() == 0)
		{
			//次のタスクを設定
			SelectTask(bossTankStatus);
		}

		//キューにタスクがある場合
		if (m_taskQueue.size() > 0)
		{
			//タスクを実行
			ExecuteTask();
		}

		//デバッグ用、現在のタスクをリセットする
		if (g_pad[0]->IsTrigger(enButtonLB2))
		{
			while (m_taskQueue.size() != 0)
			{
				m_taskQueue.front().Terminate();
				m_taskQueue.pop();
			}
		}
	}

	void BossTankBehave::CheckEvent(BossTankStatus& bossTankStatus)
	{
		//HPが半分以下になった時、行動を中断して砲身切断イベントへ
		if (bossTankStatus.isStepAttack == false && bossTankStatus.hp <= MAX_HP/2)
		{
			TerminateTask();
			bossTankStatus.isStepAttack = true;
		}
	}
	void BossTankBehave::SelectTask(BossTankStatus& bossTankStatus)
	{
		Player* player = FindGO<Player>("player");

		Vector3 distance = player->GetPosition() - bossTankStatus.position;

		//砲台を斬られてまだ雑魚敵を召喚していなかったら
		if (bossTankStatus.hp <= MAX_HP/2 && bossTankStatus.isSummonMinions == false && bossTankStatus.isCannonBreak == true)
		{
			m_taskQueue.push(m_tankTask[BossTankTasks::enSummon]);

			m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			bossTankStatus.isSummonMinions = true;
		}
		else if (bossTankStatus.hp == 0)
		{
			m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
		}
		else if (bossTankStatus.hp <= MAX_HP/2 && bossTankStatus.isCannonBreak == true)
		{
			//砲台が壊れた時
			if (distance.LengthSq() <= DISTANCE_RANGED_ATTACK * DISTANCE_RANGED_ATTACK)
			{
				//突進攻撃
				//(今はまだ回転攻撃)
				m_taskQueue.push(m_tankTask[BossTankTasks::enCharge]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			}
			else
			{
				//岩攻撃
				m_taskQueue.push(m_tankTask[BossTankTasks::enRock]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			}
		}
		else
		{
			//砲台が壊れていない時
			if (bossTankStatus.hp <= MAX_HP/2)
			{
				m_taskQueue.push(m_tankTask[BossTankTasks::enRolling]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enStep]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			}
			else if (distance.LengthSq() <= DISTANCE_RANGED_ATTACK * DISTANCE_RANGED_ATTACK)
			{
				m_taskQueue.push(m_tankTask[BossTankTasks::enRolling]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			}
			else
			{
				m_taskQueue.push(m_tankTask[BossTankTasks::enMissile]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			}
		}
	}

	void BossTankBehave::ExecuteTask()
	{
		//先頭のタスクを実行してタスクが終わったかどうかを取得
		bool isEnd = m_taskQueue.front().Execute();

		//タスクが終わったら
		if (isEnd)
		{
			//終わったタスクをキューから削除
			m_taskQueue.pop();
		}
	}

	void BossTankBehave::TerminateTask()
	{
		while (m_taskQueue.size() != 0)
			{
				m_taskQueue.front().Terminate();
				m_taskQueue.pop();
			}
	}
}
