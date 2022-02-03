#pragma once

namespace Game
{
	class CommandInput;

	class BossTankFinishCommand
	{
	public:

		/**
		 * @brief �g�h���R�}���h�̕\�����������s
		 * @param bossPosition �{�X�̍��W
		 * @param bossHp �{�X�̗̑�
		*/
		void Execute(const Vector3& bossPosition, float bossHp);

	private:
		CommandInput* m_finishCommandInput = nullptr; //�g�h���p�̃R�}���h�{�^��
	};
}
