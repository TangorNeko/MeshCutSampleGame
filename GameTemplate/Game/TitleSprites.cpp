#include "stdafx.h"
#include "TitleSprites.h"

namespace
{
	const char* SPRITE_TITLELOGO_PATH = "Assets/Image/TitleLogo.dds";
	const UINT SPRITE_TITLELOGO_WIDTH = 700;
	const UINT SPRITE_TITLELOGO_HEIGHT = 200;
	const Vector3 SPRITE_TITLELOGO_POSITION = { 200.0f,100.0f,0.0f };
	const char* SPRITE_STARTTEXT_PATH = "Assets/Image/StartText.dds";
	const UINT SPRITE_STARTTEXT_WIDTH = 320;
	const UINT SPRITE_STARTTEXT_HEIGHT = 64;
	const Vector3 SPRITE_STARTTEXT_POSITION = { 200.0f,-100.0f,0.0f };
	const Vector4 SPRITE_MULTICOLOR_OPAQUE = { 1.0f,1.0f,1.0f,1.0f };
	const Vector4 SPRITE_MULTICOLOR_TRANSPARENT = { 1.0f,1.0f,1.0f,0.0f };
	const float ALPHA_INCREASE_RATE = 0.05f;

}

namespace Game
{
	TitleSprites::~TitleSprites()
	{
		DeleteGO(m_logoRender);
		DeleteGO(m_startTextRender);
	}

	bool TitleSprites::Start()
	{
		//���S�摜���쐬
		m_logoRender = NewGO<SpriteRender>(0);
		m_logoRender->Init(SPRITE_TITLELOGO_PATH, SPRITE_TITLELOGO_WIDTH, SPRITE_TITLELOGO_HEIGHT);
		m_logoRender->SetPosition(SPRITE_TITLELOGO_POSITION);

		//�X�^�[�g�p�e�L�X�g���쐬
		m_startTextRender = NewGO<SpriteRender>(0);
		m_startTextRender->Init(SPRITE_STARTTEXT_PATH, SPRITE_STARTTEXT_WIDTH, SPRITE_STARTTEXT_HEIGHT);
		m_startTextRender->SetPosition(SPRITE_STARTTEXT_POSITION);
		return true;
	}

	void TitleSprites::Update()
	{
		//�X�^�[�g���Ă��Ȃ������牽�����Ȃ�
		if (m_isStarted == false)
		{
			return;
		}

		//���߂̐[�x�𑝂₷�B
		m_alphaProgress += ALPHA_INCREASE_RATE;

		//��Z�J���[����`���
		Vector4 mulColor;
		mulColor.Lerp(m_alphaProgress, SPRITE_MULTICOLOR_OPAQUE, SPRITE_MULTICOLOR_TRANSPARENT);

		//��Z�J���[���Z�b�g
		m_logoRender->SetMulColor(mulColor);
		m_startTextRender->SetMulColor(mulColor);

		//�����x��0�ɂȂ�����폜
		if (mulColor.w <= 0.0f)
		{
			DeleteGO(this);
		}
	}
}