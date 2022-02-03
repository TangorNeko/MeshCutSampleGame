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
		SpriteRender* m_hpFrame = nullptr;	//HP�o�[�̃t���[���̃X�v���C�g
		SpriteRender* m_hpBar = nullptr;	//HP�o�[�̃X�v���C�g
		bool m_isInited = false;			//���������ꂽ?
		float m_alpha = 0.0f;				//�����x
	};
}

