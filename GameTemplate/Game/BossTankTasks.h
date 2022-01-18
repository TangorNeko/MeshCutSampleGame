#pragma once

namespace Game
{
	class BossTank;

	class BossTankTasks
	{
	public:
		enum BossTankTask : int
		{
			enMissile,//�~�T�C���U��
			enRolling,//��]�U��
			enSummon,//�G������
			enCharge,//�ːi�U��
			enRock,//���΂�
			enStep,//����~�T�C��
			enEventRolling,//�C�x���g�p��]�U��(���s��)
			enWait,//�ҋ@
			enTaskNum
		};

		/**
		 * @brief �󂯎�����{�X�Ƀ^�X�N���Z�b�g����
		 * @param bossTank �^�X�N���Z�b�g����{�X
		*/
		void SubmitTo(BossTank* bossTank);

	private:
		/**
		 * @brief �~�T�C���𔭎˂���^�X�N��o�^����
		 * @param bossTank �o�^����{�X
		*/
		void SubmitMissileTask(BossTank* bossTank);

		/**
		 * @brief ��]�U��������^�X�N��o�^����
		 * @param bossTank �o�^����{�X
		*/
		void SubmitRollingTask(BossTank* bossTank);

		/**
		 * @brief �G���G����������^�X�N��o�^����
		 * @param bossTank �o�^����{�X
		*/
		void SubmitSummonTask(BossTank* bossTank);

		/**
		 * @brief �ːi�U��������^�X�N��o�^����
		 * @param bossTank �o�^����{�X
		*/
		void SubmitChargeTask(BossTank* bossTank);

		/**
		 * @brief ��𔭎˂���^�X�N��o�^����
		 * @param bossTank �o�^����{�X
		*/
		void SubmitRockTask(BossTank* bossTank);

		/**
		 * @brief �ҋ@����^�X�N��o�^����
		 * @param bossTank �o�^����{�X
		*/
		void SubmitWaitTask(BossTank* bossTank);

		/**
		 * @brief ����ɂȂ�~�T�C���𔭎˂���^�X�N��o�^����
		 * @param bossTank �o�^����{�X
		*/
		void SubmitStepTask(BossTank* bossTank);

		/**
		 * @brief �C�x���g�p�̉�]�U���̃^�X�N��o�^����
		 * @param bossTank �o�^����{�X
		*/
		void SubmitEventRollingTask(BossTank* bossTank);
	};
}

