#include "stdafx.h"
#include "BossTankBehave.h"
#include "Player.h"
#include "BossTank.h"


namespace
{
	const float DISTANCE_RANGED_ATTACK = 1000.0f;	//�������U�����s������
	const int MAX_HP = 400;							//�ő�̗�
}

namespace Game
{
	void BossTankBehave::SubmitTo(BossTank* bossTank)
	{
		//�{�X�Ƀ^�X�N��o�^����
		BossTankTasks bossTankTasks;
		bossTankTasks.SubmitTo(bossTank);
	}

	void BossTankBehave::Execute(BossTankStatus& bossTankStatus)
	{
		//�^�X�N���D��x�̍����C�x���g�̃`�F�b�N
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
			//�G���G������
			m_taskQueue.push(m_tankTask[BossTankTasks::enSummon]);
			m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			bossTankStatus.isSummonMinions = true;
		}
		//�{�X��HP��0���v���C���[��HP��0�̎�
		else if (bossTankStatus.hp == 0 || player->GetHp() == 0)
		{
			//�������Ȃ�
			m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
		}
		//�̗͂������ȉ��ŁA���łɖC�g���ؒf���ꂽ��
		else if (bossTankStatus.hp <= MAX_HP/2 && bossTankStatus.isCannonBreak == true)
		{
			//�ߋ����Ȃ�
			if (distance.LengthSq() <= DISTANCE_RANGED_ATTACK * DISTANCE_RANGED_ATTACK)
			{
				//��]�U��
				m_taskQueue.push(m_tankTask[BossTankTasks::enCharge]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			}
			//�������Ȃ�
			else
			{
				//��U��
				m_taskQueue.push(m_tankTask[BossTankTasks::enRock]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			}
		}
		//�C�䂪�ؒf����Ă��Ȃ���
		else
		{
			//�̗͂������ȉ��ɂȂ�����
			if (bossTankStatus.hp <= MAX_HP/2)
			{
				//�C�g�̐ؒf�C�x���g���X�^�[�g
				m_taskQueue.push(m_tankTask[BossTankTasks::enEventRolling]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enStep]);
			}
			//�ߋ����Ȃ�
			else if (distance.LengthSq() <= DISTANCE_RANGED_ATTACK * DISTANCE_RANGED_ATTACK)
			{
				//��]�U��
				m_taskQueue.push(m_tankTask[BossTankTasks::enRolling]);
				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			}
			//�������Ȃ�
			else
			{
				//�~�T�C���U��
				m_taskQueue.push(m_tankTask[BossTankTasks::enMissile]);
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
		//�^�X�N��0�ɂȂ�܂�
		while (m_taskQueue.size() != 0)
			{
				//�퓬�^�X�N�𒆒f����
				m_taskQueue.front().Terminate();
				//�L���[����폜
				m_taskQueue.pop();
			}
	}
}
