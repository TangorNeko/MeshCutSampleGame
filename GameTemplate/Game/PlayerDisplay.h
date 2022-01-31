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
		SpriteRender* m_hpFrame = nullptr;
		SpriteRender* m_hpBar = nullptr;
		bool m_isInited = false;
		float m_alpha = 0.0f;
	};
}

