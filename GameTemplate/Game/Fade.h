#pragma once

namespace Game
{
	class Fade : public IGameObject
	{
	public:
		~Fade();

		bool Start() override;

		void Update() override;

		/**
		 * @brief �t�F�[�h�C���̏���
		*/
		void FadeIn();

		/**
		 * @brief �ҋ@���̏���
		*/
		void Wait();

		/**
		 * @brief �t�F�[�h�A�E�g�̏���
		*/
		void FadeOut();

		/**
		 * @brief �t�F�[�h�C���̃��[�g���Z�b�g
		 * @param rate ���[�g
		*/
		void SetFadeInRate(float rate)
		{
			m_fadeInRate = rate;
		}
		
		/**
		 * @brief �ҋ@�t���[�����Z�b�g
		 * @param frame �ҋ@�t���[��
		*/
		void SetWaitFrame(int frame)
		{
			m_waitFrame = frame;
		}
		
		/**
		 * @brief �t�F�[�h�A�E�g�̃��[�g���Z�b�g
		 * @param rate ���[�g
		*/
		void SetFadeOutRate(float rate)
		{
			m_fadeOutRate = rate;
		}

		/**
		 * @brief �t�F�[�h�����̐i�s�x���擾
		 * @return �t�F�[�h�����̐i�s�x
		*/
		float GetFadeProgress()
		{
			return m_mulColor.w;
		}
	private:
		SpriteRender* m_fadeSprite = nullptr;			//�t�F�[�h�Ɏg�p����X�v���C�g
		Vector4 m_mulColor = { 0.0f,0.0f,0.0f,0.0f };	//�t�F�[�h�Ɏg�p����X�v���C�g�ւ̏�Z�J���[
		float m_fadeInRate = 0.01f;						//�t�F�[�h�C���̃��[�g
		int m_waitFrame = 0;							//�ҋ@����t���[��
		int m_currentWaitFrame = 0;						//�ҋ@�̌o�߃t���[��
		float m_fadeOutRate = 0.01f;					//�t�F�[�h�A�E�g�̃��[�g

		//�t�F�[�h�̐i�s�x
		enum FadeState
		{
			enFadeIn,	//�t�F�[�h�C��
			enWait,		//�ҋ@
			enFadeOut	//�t�F�[�h�A�E�g
		};

		//���݂̃t�F�[�h�̐i�s�x
		FadeState m_fadeState = enFadeIn;
	};
}
