#include "stdafx.h"
#include "Fade.h"

namespace
{
	const char* SPRITE_PATH = "Assets/Image/WhiteSprite.dds";
	const UINT SPRITE_WIDTH = 1280;
	const UINT SPRITE_HEIGHT = 720;
	const float FADERATE_MAX = 1.0f;
	const float FADERATE_MIN = 0.0f;
}

namespace Game
{
	Fade::~Fade()
	{
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
		case enFadeIn:
			FadeIn();
			break;
		case enWait:
			Wait();
			break;
		case enFadeOut:
			FadeOut();
			break;
		}

		m_fadeSprite->SetMulColor(m_mulColor);
	}


	void Fade::FadeIn()
	{
		//�����x�������������Ă���
		m_mulColor.w += m_fadeInRate;

		if (m_mulColor.w >= 1.0f)
		{
			m_mulColor.w = 1.0f;

			m_fadeState = enWait;
		}
	}

	void Fade::Wait()
	{
		//�w��t���[���ҋ@
		m_currentWaitFrame++;

		if (m_currentWaitFrame >= m_waitFrame)
		{
			m_fadeState = enFadeOut;
		}
	}

	void Fade::FadeOut()
	{
		//�����x���������グ�Ă���
		m_mulColor.w -= m_fadeOutRate;

		if (m_mulColor.w <= 0.0f)
		{
			m_mulColor.w = 0.0f;

			DeleteGO(this);
		}
	}
}
