#pragma once

namespace Game
{
	class TitleSprites : public IGameObject
	{
	public:
		~TitleSprites();

		bool Start() override;

		void Update() override;

		/**
		 * @brief �Q�[�����X�^�[�g��������ʒm����
		*/
		void NoticeGameStart()
		{
			m_isStarted = true;
		}
	private:
		SpriteRender* m_logoRender = nullptr;		//�^�C�g�����S�̃X�v���C�g
		SpriteRender* m_startTextRender = nullptr;	//�X�^�[�g�p�e�L�X�g�̃X�v���C�g
		bool m_isStarted = false;					//�Q�[�����X�^�[�g����?
		float m_alphaProgress = 0.0f;				//���߂̐[�x
	};
}

