#include "stdafx.h"
#include "BossTankDisplay.h"

namespace
{
	const int MAX_HP = 400;												//�ő�̗�
	const char* SPRITE_HPFRAME_PATH = "Assets/Image/BossHpFrame.dds";	//HP�o�[�̘g�̃X�v���C�g�p�X
	const UINT SPRITE_HPFRAME_WIDTH = 400;								//HP�o�[�̘g�̕�
	const UINT SPRITE_HPFRAME_HEIGHT = 40;								//HP�o�[�̘g�̍���
	const Vector2 SPRITE_HPFRAME_PIVOT = { 0.5f,0.0f };					//HP�o�[�̘g�̃s�{�b�g
	const Vector3 SPRITE_HPFRAME_POSITION = { 350.0f,-300.0f,0.0f };	//HP�o�[�̘g�̍��W
	const char* SPRITE_HPBAR_PATH = "Assets/Image/HpBar.dds";			//HP�o�[�̃X�v���C�g�p�X
	const UINT SPRITE_HPBAR_WIDTH = 396;								//HP�o�[�̕�
	const UINT SPRITE_HPBAR_HEIGHT = 20;								//HP�o�[�̍���
	const Vector2 SPRITE_HPBAR_PIVOT = { 0.0f,0.0f };					//HP�o�[�̃s�{�b�g
	const Vector3 SPRITE_HPBAR_POSITION = { 152.0f,-298.0f,0.0f };		//HP�o�[�̍��W
	const float SPRITE_SCALE_YZ = 1.0f;									//HP�o�[��Y��Z���̊g�嗦
}

namespace Game
{
	BossTankDisplay::~BossTankDisplay()
	{
		//HP�o�[�̉摜���폜
		DeleteGO(m_hpFrame);
		DeleteGO(m_hpBar);
	}

	void BossTankDisplay::Init(int hp)
	{
		//HP�o�[�̘g�𐶐�
		m_hpFrame = NewGO<SpriteRender>(Priority::High);
		m_hpFrame->Init(SPRITE_HPFRAME_PATH, SPRITE_HPFRAME_WIDTH, SPRITE_HPFRAME_HEIGHT);
		m_hpFrame->SetPivot(SPRITE_HPFRAME_PIVOT);
		m_hpFrame->SetPosition(SPRITE_HPFRAME_POSITION);

		//HP�o�[�𐶐�
		m_hpBar = NewGO<SpriteRender>(Priority::Middle);
		m_hpBar->Init(SPRITE_HPBAR_PATH, SPRITE_HPBAR_WIDTH, SPRITE_HPBAR_HEIGHT);
		m_hpBar->SetPivot(SPRITE_HPBAR_PIVOT);
		m_hpBar->SetPosition(SPRITE_HPBAR_POSITION);
	}

	void BossTankDisplay::Update(int hp, const Vector3& bossPosition)
	{
		//�{�X�̗̑͂ɉ�����HP�o�[�̃X�P�[����ύX����
		float hpBarScale = static_cast<float>(hp) / MAX_HP;
		m_hpBar->SetScale({ hpBarScale,SPRITE_SCALE_YZ,SPRITE_SCALE_YZ });
	}
}