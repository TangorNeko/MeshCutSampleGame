#include "stdafx.h"
#include "TitleSprites.h"

namespace
{
	const char* SPRITE_TITLELOGO_PATH = "Assets/Image/TitleLogo.dds";		//�^�C�g�����S�̃X�v���C�g�p�X
	const UINT SPRITE_TITLELOGO_WIDTH = 700;								//�^�C�g�����S�̕�
	const UINT SPRITE_TITLELOGO_HEIGHT = 200;								//�^�C�g�����S�̍���
	const Vector3 SPRITE_TITLELOGO_POSITION = { 200.0f,100.0f,0.0f };		//�^�C�g�����S�̍��W
	const char* SPRITE_STARTTEXT_PATH = "Assets/Image/StartText.dds";		//�X�^�[�g�e�L�X�g�̃X�v���C�g�p�X
	const UINT SPRITE_STARTTEXT_WIDTH = 320;								//�X�^�[�g�e�L�X�g�̕�
	const UINT SPRITE_STARTTEXT_HEIGHT = 64;								//�X�^�[�g�e�L�X�g�̍���
	const Vector3 SPRITE_STARTTEXT_POSITION = { 200.0f,-100.0f,0.0f };		//�X�^�[�g�e�L�X�g�̍��W
	const Vector4 SPRITE_MULTICOLOR_OPAQUE = { 1.0f,1.0f,1.0f,1.0f };		//�X�v���C�g�̕s�����J���[
	const Vector4 SPRITE_MULTICOLOR_TRANSPARENT = { 1.0f,1.0f,1.0f,0.0f };	//�X�v���C�g�̓��߃J���[
	const float ALPHA_INCREASE_RATE = 0.05f;								//�X�v���C�g�̓��ߐi�s�x

}

namespace Game
{
	TitleSprites::~TitleSprites()
	{
		//�^�C�g�����S�ƃX�^�[�g�e�L�X�g���폜
		DeleteGO(m_logoRender);
		DeleteGO(m_startTextRender);
	}

	bool TitleSprites::Start()
	{
		//���S�摜���쐬
		m_logoRender = NewGO<SpriteRender>(Priority::High);
		m_logoRender->Init(SPRITE_TITLELOGO_PATH, SPRITE_TITLELOGO_WIDTH, SPRITE_TITLELOGO_HEIGHT);
		m_logoRender->SetPosition(SPRITE_TITLELOGO_POSITION);

		//�X�^�[�g�p�e�L�X�g���쐬
		m_startTextRender = NewGO<SpriteRender>(Priority::High);
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