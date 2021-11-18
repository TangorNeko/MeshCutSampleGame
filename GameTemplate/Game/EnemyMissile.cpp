#include "stdafx.h"
#include "EnemyMissile.h"
#include "CutDummy.h"

namespace
{
	const char* PATH_MISSILEMODEL = "Assets/modelData/TankMissile.tkm";
	const wchar_t* CANCUT_TEXT = L"Can Cut";
	const Vector4 SHADOWCOLOR_BLACK = { 0.0f,0.0f,0.0f,1.0f };
}

namespace Game
{
	EnemyMissile::~EnemyMissile()
	{
		if (m_missileRender != nullptr && m_isCut == false)
		{
			DeleteGO(m_missileRender);
		}

		if (m_canCutTextRender != nullptr)
		{
			DeleteGO(m_canCutTextRender);
		}
	}

	bool EnemyMissile::Start()
	{
		m_missileRender = NewGO<SkinModelRender>(0);
		m_missileRender->Init(PATH_MISSILEMODEL);
		m_missileRender->SetOwner(this);

		return true;
	}

	void EnemyMissile::Update()
	{
		m_position.x += 1.0f;
		m_missileRender->SetPosition(m_position);
		m_missileRender->SetRotation(m_qRot);

		if (m_hp <= 0)
		{
			Vector2 screenPos;
			g_camera3D->CalcScreenPositionFromWorldPosition(screenPos, m_position);

			m_canCutTextRender->SetPosition(screenPos);
		}
	}

	void EnemyMissile::OnDivide(const SkinModelRender* skinModelRender)
	{
		OutputDebugStringA("OnDivide");
		m_missileRender->MakeDummy();
		m_isCut = true;

		DeleteGO(this);
	}

	void EnemyMissile::Damage(float damage)
	{
		if (m_isCut == false)
		{
			if (m_hp > 0)
			{
				m_hp -= damage;

				if (m_hp <= 0)
				{
					m_hp = 0;

					m_missileRender->SetDivideFlag(true);
					Game::ModelCutManager::GetInstance()->AddCuttable(m_missileRender);

					m_canCutTextRender = NewGO<FontRender>(0);
					m_canCutTextRender->SetText(CANCUT_TEXT);
					m_canCutTextRender->SetShadowFlag(true);
					m_canCutTextRender->SetShadowColor(SHADOWCOLOR_BLACK);
				}
			}
		}
	}
}