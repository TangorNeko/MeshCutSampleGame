#include "stdafx.h"
#include "PlayerDisplay.h"

namespace
{
	const float MAX_HP = 1000.0f;									//�ő�̗�
	const char* HP_FRAME_PATH = "Assets/Image/PlayerHpFrame.dds";	//HP�o�[�̘g�̃X�v���C�g�p�X
	const UINT HP_FRAME_WIDTH = 400;								//HP�o�[�̘g�̕�
	const UINT HP_FRAME_HEIGHT = 40;								//HP�o�[�̘g�̍���
	const Vector3 HP_FRAME_POSITION = { -350.0f,275.0f,0.0f };		//HP�o�[�̘g�̍��W
	const Vector2 HP_FRAME_PIVOT = { 0.5f,0.0f };					//HP�o�[�̘g�̃s�{�b�g
	const char* HP_BAR_PATH = "Assets/Image/HpBar.dds";				//HP�o�[�̃X�v���C�g�p�X
	const UINT HP_BAR_WIDTH = 396;									//HP�o�[�̕�
	const UINT HP_BAR_HEIGHT = 20;									//HP�o�[�̍���
	const Vector3 HP_BAR_POSITION = { -548.0f,277.0f,0.0f };		//HP�o�[�̍��W
	const Vector2 HP_BAR_PIVOT = { 0.0f,0.0f };						//HP�o�[�̃s�{�b�g
	const float ALPHA_INCREASE_VALUE = 0.01f;						//HP�o�[�̓����x�̑�����
	const float ALPHA_MAX = 1.0f;									//HP�o�[�̓����x�̍ő�l
	const float HPBAR_SCALE_YZ = 1.0f;								//HP�o�[��Y��Z���̊g�嗦
}

namespace Game
{
	PlayerDisplay::~PlayerDisplay()
	{
		//�̗͂̉摜���폜
		DeleteGO(m_hpFrame);
		DeleteGO(m_hpBar);
	}

	void PlayerDisplay::Init(int hp)
	{
		//HP�o�[�̘g�𐶐�
		m_hpFrame = NewGO<SpriteRender>(Priority::High);
		m_hpFrame->Init(HP_FRAME_PATH, HP_FRAME_WIDTH, HP_FRAME_HEIGHT);
		m_hpFrame->SetPivot(HP_FRAME_PIVOT);
		m_hpFrame->SetPosition(HP_FRAME_POSITION);

		//HP�o�[�𐶐�
		m_hpBar = NewGO<SpriteRender>(Priority::Middle);
		m_hpBar->Init(HP_BAR_PATH, HP_BAR_WIDTH, HP_BAR_HEIGHT);
		m_hpBar->SetPivot(HP_BAR_PIVOT);
		m_hpBar->SetPosition(HP_BAR_POSITION);

		//�������I��
		m_isInited = true;
	}

	void PlayerDisplay::Update(int hp)
	{
		//����������Ă��Ȃ��Ȃ牽�����Ȃ�
		if (m_isInited == false)
		{
			return;
		}

		//�����x�̒l���グ�Ă���(�����x�������Ă���)
		m_alpha = min(ALPHA_MAX, m_alpha + ALPHA_INCREASE_VALUE);
		Vector4 mulColor = Vector4::White;
		mulColor.w = m_alpha;

		//HP�o�[�ɓ����x���Z�b�g
		m_hpFrame->SetMulColor(mulColor);
		m_hpBar->SetMulColor(mulColor);
	
		//HP�ɉ����Ċg�嗦���Z�b�g
		m_hpBar->SetScale({ hp / MAX_HP,HPBAR_SCALE_YZ,HPBAR_SCALE_YZ });
	}
}