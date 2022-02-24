#include "stdafx.h"
#include "Fade.h"

namespace
{
	const char* SPRITE_PATH = "Assets/Image/WhiteSprite.dds";	//�t�F�[�h�̃X�v���C�g�p�X
	const UINT SPRITE_WIDTH = 1280;								//�t�F�[�h�̃X�v���C�g�̕�
	const UINT SPRITE_HEIGHT = 720;								//�t�F�[�h�̃X�v���C�g�̍���
	const float FADERATE_MAX = 1.0f;							//�t�F�[�h�̍ő�l
	const float FADERATE_MIN = 0.0f;							//�t�F�[�h�̍ŏ��l
}

namespace Game
{
	Fade::~Fade()
	{
		//�t�F�[�h�p�̃X�v���C�g���폜
		DeleteGO(m_fadeSprite);
	}

	bool Fade::Start()
	{
		//�t�F�[�h�p�̃X�v���C�g�𐶐�
		m_fadeSprite = NewGO<SpriteRender>(Priority::Low);
		m_fadeSprite->Init(SPRITE_PATH, SPRITE_WIDTH, SPRITE_HEIGHT);
		m_fadeSprite->SetMulColor(m_mulColor);
		return true;
	}

	void Fade::Update()
	{
		//�t�F�[�h�̐i�s�x�ŕ���
		switch (m_fadeState)
		{
		case enFadeIn://�t�F�[�h�C��
			FadeIn();
			break;
		case enWait://�ҋ@
			Wait();
			break;
		case enFadeOut://�t�F�[�h�A�E�g
			FadeOut();
			break;
		}

		//�����x���Z�b�g
		m_fadeSprite->SetMulColor(m_mulColor);
	}


	void Fade::FadeIn()
	{
		//�����x�������������Ă���
		m_mulColor.w += m_fadeInRate;

		//�����x��1�ȏ�Ȃ�
		if (m_mulColor.w >= 1.0f)
		{
			//1�ɂ���
			m_mulColor.w = 1.0f;

			//�t�F�[�h�C�����I�������̂őҋ@��Ԃ�
			m_fadeState = enWait;
		}
	}

	void Fade::Wait()
	{
		//�w��t���[���ҋ@
		m_currentWaitFrame++;

		//�ҋ@�t���[���𒴂����
		if (m_currentWaitFrame >= m_waitFrame)
		{
			//�t�F�[�h�A�E�g�ֈڍs
			m_fadeState = enFadeOut;
		}
	}

	void Fade::FadeOut()
	{
		//�����x���������グ�Ă���
		m_mulColor.w -= m_fadeOutRate;

		//�����x��0�ȉ��ɂȂ��
		if (m_mulColor.w <= 0.0f)
		{
			//0�ɂ���
			m_mulColor.w = 0.0f;

			//�t�F�[�h�A�E�g���I�������̂ŃC���X�^���X���폜
			DeleteGO(this);
		}
	}
}
