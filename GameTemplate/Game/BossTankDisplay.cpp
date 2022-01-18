#include "stdafx.h"
#include "BossTankDisplay.h"

namespace
{
	const wchar_t* CANNONCUT_TEXT = L"Can Cut Cannon";
	const wchar_t* ALLCUT_TEXT = L"Can Cut All";
	const Vector4 SHADOWCOLOR_BLACK = { 0.0f,0.0f,0.0f,1.0f };
	const Vector2 HP_FONT_POSITION = { 350.0f,-300.0f };
	const int MAX_HP = 400;
}

namespace Game
{
	BossTankDisplay::~BossTankDisplay()
	{
		//HPのテキストを削除
		/*
		DeleteGO(m_hpRender);
		*/

		//体力の画像を削除
		DeleteGO(m_hpFrame);
		DeleteGO(m_hpBar);

		//切断状態のテキストを削除
		DeleteGO(m_cutStateRender);
	}

	void BossTankDisplay::Init(int hp)
	{
		m_hpFrame = NewGO<SpriteRender>(2);
		m_hpFrame->Init("Assets/Image/BossHpFrame.dds", 400, 40);
		m_hpFrame->SetPivot({ 0.5f,0.0f });
		m_hpFrame->SetPosition({ 350.0f,-300.0f,0.0f });

		m_hpBar = NewGO<SpriteRender>(3);
		m_hpBar->Init("Assets/Image/HpBar.dds", 396, 20);
		m_hpBar->SetPivot({ 0.0f,0.0f });
		m_hpBar->SetPosition({ 152.0f,-298.0f,0.0f });

		/*
		m_hpRender = NewGO<FontRender>(1);
		wchar_t buffer[256];
		swprintf_s(buffer, L"BOSSHP:%d", hp);
		m_hpRender->SetText(buffer);
		m_hpRender->SetPosition(HP_FONT_POSITION);
		m_hpRender->SetShadowFlag(true);
		m_hpRender->SetShadowColor(SHADOWCOLOR_BLACK);
		*/
	}

	void BossTankDisplay::Update(int hp, const Vector3& bossPosition)
	{
		/*
		wchar_t buffer[256];
		swprintf_s(buffer, L"BOSSHP:%d", hp);
		m_hpRender->SetText(buffer);
		*/

		if (hp <= MAX_HP/2)
		{
			if (m_cutStateRender == nullptr)
			{
				/*
				m_cutStateRender = NewGO<FontRender>(0);
				m_cutStateRender->SetText(CANNONCUT_TEXT);
				m_cutStateRender->SetShadowFlag(true);
				m_cutStateRender->SetShadowColor(SHADOWCOLOR_BLACK);
				*/
			}
		}
		
		if(hp <= 0)
		{
			//m_cutStateRender->SetText(ALLCUT_TEXT);
		}

		if (hp <= MAX_HP/2)
		{
			/*
			Vector2 screenPos;
			g_camera3D->CalcScreenPositionFromWorldPosition(screenPos, bossPosition);
			m_cutStateRender->SetPosition(screenPos);
			*/
		}


		float hpBarScale = static_cast<float>(hp) / MAX_HP;
		m_hpBar->SetScale({ hpBarScale,1.0f,1.0f });

	}
}