#pragma once

namespace Game
{
	class PlayerDisplay
	{
	public:
		~PlayerDisplay();

		/**
		 * @brief �v���C���[�̏��\���̏�����
		 * @param hp �v���C���[�̗̑�
		*/
		void Init(int hp);

		/**
		 * @brief �v���C���[�̏��\���̃A�b�v�f�[�g
		 * @param hp �v���C���[�̗̑�
		*/
		void Update(int hp);
	private:
		FontRender* m_hpRender = nullptr;	//HP��`�悷��t�H���g
	};
}
