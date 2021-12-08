#include "stdafx.h"
#include "BossTankDisplay.h"

namespace
{
	const wchar_t* CANNONCUT_TEXT = L"Can Cut Cannon";
	const wchar_t* ALLCUT_TEXT = L"Can Cut All";
	const Vector4 SHADOWCOLOR_BLACK = { 0.0f,0.0f,0.0f,1.0f };
	const Vector2 HP_FONT_POSITION = { 350.0f,-300.0f };
}

namespace Game
{
	BossTankDisplay::~BossTankDisplay()
	{
		//HPのテキストを削除
		DeleteGO(m_hpRender);

		//切断状態のテキストを削除
		DeleteGO(m_cutStateRender);
	}

	void BossTankDisplay::Init(int hp)
	{
		m_hpRender = NewGO<FontRender>(1);
		wchar_t buffer[256];
		swprintf_s(buffer, L"BOSSHP:%d", hp);
		m_hpRender->SetText(buffer);
		m_hpRender->SetPosition(HP_FONT_POSITION);
		m_hpRender->SetShadowFlag(true);
		m_hpRender->SetShadowColor(SHADOWCOLOR_BLACK);
	}

	void BossTankDisplay::Update(int hp, const Vector3& bossPosition)
	{
		wchar_t buffer[256];
		swprintf_s(buffer, L"BOSSHP:%d", hp);
		m_hpRender->SetText(buffer);

		if (hp <= 500.0f)
		{
			if (m_cutStateRender == nullptr)
			{
				m_cutStateRender = NewGO<FontRender>(0);
				m_cutStateRender->SetText(CANNONCUT_TEXT);
				m_cutStateRender->SetShadowFlag(true);
				m_cutStateRender->SetShadowColor(SHADOWCOLOR_BLACK);
			}
		}
		
		if(hp <= 0)
		{
			m_cutStateRender->SetText(ALLCUT_TEXT);
		}

		if (hp <= 500.0f)
		{
			Vector2 screenPos;
			g_camera3D->CalcScreenPositionFromWorldPosition(screenPos, bossPosition);
			m_cutStateRender->SetPosition(screenPos);
		}

	}
}