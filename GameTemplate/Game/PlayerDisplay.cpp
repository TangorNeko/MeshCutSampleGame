#include "stdafx.h"
#include "PlayerDisplay.h"

namespace
{
	const Vector4 SHADOWCOLOR_BLACK = { 0.0f,0.0f,0.0f,1.0f };
	const Vector2 HP_POSITION = { -600.0f,330.0f };
}

namespace Game
{
	PlayerDisplay::~PlayerDisplay()
	{
		//�t�H���g�����_�[���폜
		DeleteGO(m_hpRender);
	}

	void PlayerDisplay::Init(int hp)
	{
		//�t�H���g�����_�[���쐬
		m_hpRender = NewGO<FontRender>(1);

		//HP�𕶎���ɕϊ�
		wchar_t buffer[256];
		swprintf_s(buffer, L"HP:%d", hp);

		//��������Z�b�g
		m_hpRender->SetText(buffer);
		
		//������ݒ�
		m_hpRender->SetShadowFlag(true);
		m_hpRender->SetShadowColor(SHADOWCOLOR_BLACK);
	
		//���W���Z�b�g
		m_hpRender->SetPosition(HP_POSITION);
	}

	void PlayerDisplay::Update(int hp)
	{
		//HP�𕶎���ɕϊ�
		wchar_t buffer[256];
		swprintf_s(buffer, L"HP:%d", hp);

		//��������X�V
		m_hpRender->SetText(buffer);
	}
}