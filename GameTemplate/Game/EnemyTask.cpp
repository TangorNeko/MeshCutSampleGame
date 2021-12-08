#include "stdafx.h"
#include "EnemyTask.h"

namespace Game
{
	void EnemyTask::TaskStart()
	{
		//�X�^�[�g���ɌĂ΂��֐������s����
		m_taskStartFunc();

		//�^�X�N�̏�Ԃ��A�b�v�f�[�g��ԂɑJ��
		m_taskState = enUpdate;
	}

	void EnemyTask::TaskUpdate()
	{
		//�A�b�v�f�[�g�ŌĂ΂��֐������s���A�I���������ǂ������󂯎��
		bool isEnd = m_taskUpdateFunc(m_taskTime);

		//�^�X�N�̏I�����󂯎������A�^�X�N�̏�Ԃ��I����ԂɑJ��
		if (isEnd == true)
		{
			m_taskState = enEnd;
		}

		//�o�߃t���[�������Z
		m_taskTime++;
	}

	void EnemyTask::TaskEnd()
	{
		//�^�X�N�̏I�����ɌĂ΂��֐������s����
		m_taskEndFunc();

		//�^�X�N�̏�Ԃ��^�X�N���甲�����ԂɑJ��
		m_taskState = enExit;
	}

	bool EnemyTask::Execute()
	{
		//�^�X�N�̏�Ԃɂ���ĕ���
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
			//�^�X�N���甲����
			return true;
		}

		return false;
	}

	void EnemyTask::Terminate()
	{
		//�^�X�N���A�b�v�f�[�g�����I������������(�X�^�[�g���̏������Ă΂�Ă�����)
		if (m_taskState == enUpdate || m_taskState == enEnd)
		{
			//�I�����̏������Ă�
			TaskEnd();
		}

		//�^�X�N�̏�Ԃ��^�X�N���甲�����ԂɑJ��
		m_taskState = enExit;
	}
}