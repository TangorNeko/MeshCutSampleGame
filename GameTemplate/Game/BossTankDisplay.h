#pragma once

namespace Game
{
	class BossTankDisplay
	{
	public:
		~BossTankDisplay();

		/**
		 * @brief ���\���̏�����
		 * @param hp �{�X�̗̑�
		*/
		void Init(int hp);

		/**
		 * @brief ���\���̍X�V
		 * @param hp �{�X�̗̑�
		 * @param bossPosition �{�X�̍��W 
		*/
		void Update(int hp,const Vector3& bossPosition);

	private:
		SpriteRender* m_hpFrame = nullptr;	//HP�o�[�̘g�̃X�v���C�g
		SpriteRender* m_hpBar = nullptr;	//HP�o�[�̃X�v���C�g
	};
}