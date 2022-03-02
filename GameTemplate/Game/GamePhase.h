#pragma once

namespace Game
{
	class IPhase;

	class GamePhase
	{
	public:
		~GamePhase();

		/**
		 * @brief �e�t�F�[�Y�̏�����
		*/
		void Init();

		/**
		 * @brief �t�F�[�Y�̃A�b�v�f�[�g
		*/
		void Update();

		/**
		 * @brief ���̃t�F�[�Y�ɐi�s������
		*/
		void Progress();

		/**
		 * @brief �w�肵���ԍ��̃t�F�[�Y�ɃX�L�b�v������
		 * @param phaseNo �t�F�[�Y�ԍ�
		*/
		void SkipTo(int phaseNo);
	
	private:
		std::vector<IPhase*> m_phaseVector;	//�t�F�[�Y�̔z��
		IPhase* m_currentPhase = nullptr;	//���݂̃t�F�[�Y
		int m_currentPhaseNo = 0;			//���݂̃t�F�[�Y�ԍ�
	};
}

