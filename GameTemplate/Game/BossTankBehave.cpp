#include "stdafx.h"
#include "BossTankBehave.h"
#include "Player.h"
#include "BossTank.h"


namespace
{
	const float DISTANCE_RANGED_ATTACK = 1000.0f;	//遠距離攻撃を行う距離
	const int MAX_HP = 400;							//最大体力
}

namespace Game
{
	void BossTankBehave::SubmitTo(BossTank* bossTank)
	{
		//ボスにタスクを登録する
		BossTankTasks bossTankTasks;
		bossTankTasks.SubmitTo(bossTank);
	}

	void BossTankBehave::Execute(BossTankStatus& bossTankStatus)
	{
		//タスクより優先度の高いイベントのチェック
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
			//雑魚敵を召喚
			m_taskQueue.push(m_tankTask[BossTankTasks::enSummon]);
			m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			bossTankStatus.isSummonMinions = true;
		}
		//ボスのHPが0かプレイヤーのHPが0の時
		else if (bossTankStatus.hp == 0 || player->GetHp() == 0)
		{
			//何もしない
			m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
		}
		//体力が半分以下で、すでに砲身が切断された時
		else if (bossTankStatus.hp <= MAX_HP/2 && bossTankStatus.isCannonBreak == true)
		{
			//近距離なら
			if (distance.LengthSq() <= DISTANCE_RANGED_ATTACK * DISTANCE_RANGED_ATTACK)
			{
				//回転攻撃
				m_taskQueue.push(m_tankTask[BossTankTasks::enCharge]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			}
			//遠距離なら
			else
			{
				//岩攻撃
				m_taskQueue.push(m_tankTask[BossTankTasks::enRock]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			}
		}
		//砲台が切断されていない時
		else
		{
			//体力が半分以下になったら
			if (bossTankStatus.hp <= MAX_HP/2)
			{
				//砲身の切断イベントをスタート
				m_taskQueue.push(m_tankTask[BossTankTasks::enEventRolling]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enStep]);
			}
			//近距離なら
			else if (distance.LengthSq() <= DISTANCE_RANGED_ATTACK * DISTANCE_RANGED_ATTACK)
			{
				//回転攻撃
				m_taskQueue.push(m_tankTask[BossTankTasks::enRolling]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			}
			//遠距離なら
			else
			{
				//ミサイル攻撃
				m_taskQueue.push(m_tankTask[BossTankTasks::enMissile]);
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
		//タスクが0になるまで
		while (m_taskQueue.size() != 0)
			{
				//戦闘タスクを中断する
				m_taskQueue.front().Terminate();
				//キューから削除
				m_taskQueue.pop();
			}
	}
}
