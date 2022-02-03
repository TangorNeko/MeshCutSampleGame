#pragma once

namespace Game
{
	class DashTutorial : public IGameObject
	{
		~DashTutorial();

		bool Start() override;

		void Update() override;

	private:
		SpriteRender* m_spriteRender = nullptr; //�`���[�g���A���̃X�v���C�g
		int m_popUpProgress = 0;				//�|�b�v�A�b�v�����̐i�x
	};
}