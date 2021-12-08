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
		FontRender* m_hpRender = nullptr;			//�̗͂�\������t�H���g
		FontRender* m_cutStateRender = nullptr;		//�ؒf�󋵂�\������t�H���g
	};
}