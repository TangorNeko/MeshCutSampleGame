#include "stdafx.h"
#include "PlayerDisplay.h"

namespace
{
	const Vector4 SHADOWCOLOR_BLACK = { 0.0f,0.0f,0.0f,1.0f };
	const char* HP_FRAME_PATH = "Assets/Image/PlayerHpFrame.dds";
	const UINT HP_FRAME_WIDTH = 400;
	const UINT HP_FRAME_HEIGHT = 40;
	const Vector3 HP_FRAME_POSITION = { -350.0f,275.0f,0.0f };
	const Vector2 HP_FRAME_PIVOT = { 0.5f,0.0f };
	const char* HP_BAR_PATH = "Assets/Image/HpBar.dds";
	const UINT HP_BAR_WIDTH = 396;
	const UINT HP_BAR_HEIGHT = 20;
	const Vector3 HP_BAR_POSITION = { -548.0f,277.0f,0.0f };
	const Vector2 HP_BAR_PIVOT = { 0.0f,0.0f };
	const float ALPHA_INCREASE_VALUE = 0.01f;
	const float ALPHA_MAX = 1.0f;
	const float HPBAR_SCALE_YZ = 1.0f;
	const float HPBAR_SCALE_DIVIDENUM = 1000.0f;
}

namespace Game
{
	PlayerDisplay::~PlayerDisplay()
	{
		//体力の画像を削除
		DeleteGO(m_hpFrame);
		DeleteGO(m_hpBar);

		//フォントレンダーを削除
		/*
		DeleteGO(m_hpRender);
		*/
	}

	void PlayerDisplay::Init(int hp)
	{
		m_hpFrame = NewGO<SpriteRender>(2);
		m_hpFrame->Init(HP_FRAME_PATH, HP_FRAME_WIDTH, HP_FRAME_HEIGHT);
		m_hpFrame->SetPivot(HP_FRAME_PIVOT);
		m_hpFrame->SetPosition(HP_FRAME_POSITION);

		m_hpBar = NewGO<SpriteRender>(3);
		m_hpBar->Init(HP_BAR_PATH, HP_BAR_WIDTH, HP_BAR_HEIGHT);
		m_hpBar->SetPivot(HP_BAR_PIVOT);
		m_hpBar->SetPosition(HP_BAR_POSITION);

		m_isInited = true;
	}

	void PlayerDisplay::Update(int hp)
	{
		if (m_isInited == false)
		{
			return;
		}


		m_alpha = min(ALPHA_MAX, m_alpha + ALPHA_INCREASE_VALUE);
		Vector4 mulColor = Vector4::White;
		mulColor.w = m_alpha;

		m_hpFrame->SetMulColor(mulColor);
		m_hpBar->SetMulColor(mulColor);
	

		m_hpBar->SetScale({ hp / HPBAR_SCALE_DIVIDENUM,HPBAR_SCALE_YZ,HPBAR_SCALE_YZ });
	}
}