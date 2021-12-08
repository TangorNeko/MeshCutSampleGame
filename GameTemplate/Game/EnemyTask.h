#pragma once

namespace Game
{
	class EnemyTask
	{
	public:
		/**
		 * @brief �^�X�N�̃X�^�[�g���ɌĂ΂��֐����Z�b�g����
		 * @param startFunc �^�X�N�̃X�^�[�g���ɌĂ΂��֐�
		*/
		void SetStartFunc(std::function<void()> startFunc)
		{
			m_taskStartFunc = startFunc;
		}

		/**
		 * @brief �^�X�N�̃A�b�v�f�[�g���ɌĂ΂��֐����Z�b�g����
		 * @param updateFunc �^�X�N�̃A�b�v�f�[�g���ɌĂ΂��֐�
		*/
		void SetUpdateFunc(std::function<bool(int)> updateFunc)
		{
			m_taskUpdateFunc = updateFunc;
		}

		/**
		 * @brief �^�X�N�̏I�����ɌĂ΂��֐����Z�b�g����
		 * @param endFunc �^�X�N�̏I�����ɌĂ΂��֐�
		*/
		void SetEndFunc(std::function<void()> endFunc)
		{
			m_taskEndFunc = endFunc;
		}

		/**
		 * @brief �^�X�N�X�^�[�g���̏���
		*/
		void TaskStart();

		/**
		 * @brief �^�X�N�A�b�v�f�[�g���̏���
		*/
		void TaskUpdate();

		/**
		 * @brief �^�X�N�I�����̏���
		*/
		void TaskEnd();

		/**
		 * @brief �^�X�N�̎��s
		 * @return �^�X�N���I������?
		*/
		bool Execute();

		/**
		 * @brief �^�X�N�̋����I��
		*/
		void Terminate();
	private:
		enum TaskState
		{
			enStart,	//�^�X�N�X�^�[�g
			enUpdate,	//�^�X�N�A�b�v�f�[�g
			enEnd,		//�^�X�N�I��
			enExit		//�^�X�N���甲����
		};
		std::function<void()> m_taskStartFunc = []() {};								//�^�X�N�̃X�^�[�g���ɌĂ΂��֐�
		std::function<bool(int)> m_taskUpdateFunc = [](int taskTime) { return true;};	//�^�X�N�̃A�b�v�f�[�g���ɌĂ΂��֐�
		std::function<void()> m_taskEndFunc = []() {};									//�^�X�N�̏I�����ɌĂ΂��֐�
		TaskState m_taskState = enStart;												//�^�X�N�̏��
		int m_taskTime = 0;																//�^�X�N�̌o�߃t���[��
	};
}

