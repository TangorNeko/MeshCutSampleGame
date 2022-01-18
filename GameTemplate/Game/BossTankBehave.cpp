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

		//�L���[�Ƀ^�X�N���Ȃ��ꍇ
		if (m_taskQueue.size() == 0)
		{
			//���̃^�X�N��ݒ�
			SelectTask(bossTankStatus);
		}

		//�L���[�Ƀ^�X�N������ꍇ
		if (m_taskQueue.size() > 0)
		{
			//�^�X�N�����s
			ExecuteTask();
		}

		//�f�o�b�O�p�A���݂̃^�X�N�����Z�b�g����
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
		//HP�������ȉ��ɂȂ������A�s���𒆒f���ĖC�g�ؒf�C�x���g��
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

		//�C����a���Ă܂��G���G���������Ă��Ȃ�������
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
			//�C�䂪��ꂽ��
			if (distance.LengthSq() <= DISTANCE_RANGED_ATTACK * DISTANCE_RANGED_ATTACK)
			{
				//�ːi�U��
				//(���͂܂���]�U��)
				m_taskQueue.push(m_tankTask[BossTankTasks::enCharge]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			}
			else
			{
				//��U��
				m_taskQueue.push(m_tankTask[BossTankTasks::enRock]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			}
		}
		else
		{
			//�C�䂪���Ă��Ȃ���
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
		//�擪�̃^�X�N�����s���ă^�X�N���I��������ǂ������擾
		bool isEnd = m_taskQueue.front().Execute();

		//�^�X�N���I�������
		if (isEnd)
		{
			//�I������^�X�N���L���[����폜
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
