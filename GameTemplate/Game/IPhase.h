#pragma once

namespace Game
{
	class IPhase
	{
	public:

		//�t�F�[�Y�̏��
		enum PhaseStatus
		{
			BeforeStart,	//�X�^�[�g�O
			StartFrame,		//�X�^�[�g�t���[��
			AfterStart,		//�X�^�[�g��
		};

		/**
		 * @brief �t�F�[�Y�̏���������
		*/
		virtual void Init() {}

		/**
		 * @brief �t�F�[�Y�̊J�n������
		*/
		virtual void PhaseStart() {}

		bool Update()
		{
			//�t�F�[�Y�̏�Ԃŕ���
			switch(GetPhaseStatus())
			{
			case BeforeStart://�X�^�[�g�O
				return false;
				break;
			case StartFrame://�X�^�[�g�t���[��
				PhaseStart();
				return false;
				break;
			case AfterStart://�X�^�[�g��
				return PhaseUpdate();
			}

			return false;
		}

		/**
		 * @brief �t�F�[�Y�̍X�V����
		 * @return �t�F�[�Y���I������?
		*/
		virtual bool PhaseUpdate() = 0;

		/**
		 * @brief �t�F�[�Y���X�^�[�g����t���[�����Z�b�g����
		 * @param phaseStartFrame �t�F�[�Y���X�^�[�g����t���[�� 
		*/
		void SetPhaseStartFrame(int phaseStartFrame)
		{
			m_phaseStartFrame = phaseStartFrame;
		}

		/**
		 * @brief �t�F�[�Y�̃t���[�����𑝉�������
		*/
		void PhaseTimeProgress()
		{
			m_phaseWaitFrame++;
		}

		/**
		 * @brief �t�F�[�Y�̃t���[�����𑝉������A�t�F�[�Y�̏�Ԃ��擾����
		 * @return �t�F�[�Y�̏��
		*/
		PhaseStatus GetPhaseStatus()
		{
			//�t�F�[�Y�̃t���[�����𑝉�������
			PhaseTimeProgress();

			//�X�^�[�g�t���[���Ɠ����Ȃ�
			if (m_phaseWaitFrame == m_phaseStartFrame)
			{
				//�X�^�[�g�t���[��
				return StartFrame;
			}
			//�X�^�[�g�t���[���ȍ~�Ȃ�
			else if (m_phaseWaitFrame > m_phaseStartFrame)
			{
				//�X�^�[�g��
				return AfterStart;
			}

			//�X�^�[�g�O
			return BeforeStart;
		}

		/**
		 * @brief �o�߂����t���[�������擾
		 * @return �o�߂����t���[����
		*/
		int GetPhaseWaitFrame()
		{
			return m_phaseWaitFrame;
		}

	private:
		int m_phaseWaitFrame = 0;	//�t�F�[�Y���X�^�[�g����܂łɑҋ@�����t���[����
		int m_phaseStartFrame = 0;	//�t�F�[�Y���X�^�[�g����t���[��
	};
}