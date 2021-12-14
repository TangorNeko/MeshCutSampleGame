#pragma once
#include "BossTankStatus.h"
#include "EnemyTask.h"
#include "BossTankTasks.h"

namespace Game
{
	class BossTank;

	class BossTankBehave
	{
	public:
		/**
		 * @brief �{�X�̃C���X�^���X�Ƀ^�X�N��o�^����
		 * @param bossTank �{�X�̃C���X�^���X�̃|�C���^
		*/
		void SubmitTo(BossTank* bossTank);

		/**
		 * @brief �^�X�N��o�^����
		 * @param taskNum �^�X�N�̔ԍ�
		 * @param task �o�^����^�X�N
		*/
		void SetTask(int taskNum, EnemyTask task)
		{
			m_tankTask[taskNum] = task;
		}

		/**
		 * @brief �{�X�̍s�������s
		 * @param bossTankStatus �s������Ɏg���{�X�̃X�e�[�^�X 
		*/
		void Execute(BossTankStatus& bossTankStatus);

		/**
		 * @brief ���s���̃^�X�N���D�悵�čs����C�x���g���`�F�b�N����
		 * @param bossTankStatus �s������Ɏg���{�X�̃X�e�[�^�X
		*/
		void CheckEvent(BossTankStatus& bossTankStatus);

		/**
		 * @brief ���̃^�X�N��I������
		 * @param bossTankStatus �I���Ɏg���{�X�̃X�e�[�^�X
		*/
		void SelectTask(BossTankStatus& bossTankStatus);

		/**
		 * @brief �^�X�N�����s����
		*/
		void ExecuteTask();

		void TerminateTask();
	private:
		EnemyTask m_tankTask[BossTankTasks::enTaskNum];		//�{�X�̃^�X�N�̔z��(���{�A��������L���[�ɃR�s�[�����)
		std::queue<EnemyTask> m_taskQueue;					//�{�X�̃^�X�N�L���[
	};
}

