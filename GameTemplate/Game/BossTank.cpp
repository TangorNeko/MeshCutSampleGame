#include "stdafx.h"
#include "BossTank.h"

namespace
{
	const char* MODEL_PATH_BASE = "Assets/modelData/TankBase.tkm";
	const char* MODEL_PATH_TURRET = "Assets/modelData/TankTurret.tkm";
	const char* MODEL_PATH_CANNON = "Assets/modelData/TankCannon.tkm";
	const wchar_t* CANNONCUT_TEXT = L"Can Cut Cannon";
	const wchar_t* ALLCUT_TEXT = L"Can Cut All";
	const Vector4 SHADOWCOLOR_BLACK = { 0.0f,0.0f,0.0f,1.0f };
}

namespace Game
{
	BossTank::~BossTank()
	{
		DeleteGO(m_baseRender);
		DeleteGO(m_turretRender);
		DeleteGO(m_cannonRender);
	}

	bool BossTank::Start()
	{
		m_baseRender = NewGO<SkinModelRender>(0);
		m_turretRender = NewGO<SkinModelRender>(0);
		m_cannonRender = NewGO<SkinModelRender>(0);


		m_baseRender->Init(MODEL_PATH_BASE);
		m_turretRender->Init(MODEL_PATH_TURRET);
		m_cannonRender->Init(MODEL_PATH_CANNON);
		m_cannonRender->SetOwner(this);

		return true;
	}

	void BossTank::OnDivide()
	{
		//TODO:砲身が壊れたフラグ、行動を分岐させるのに使う
		m_isCannonBreak = true;
	}

	void BossTank::Update()
	{
		m_turretRot.SetRotationDegY(m_turretDeg);

		m_turretRot.Multiply(m_baseRot, m_turretRot);

		m_baseRender->SetRotation(m_baseRot);
		m_turretRender->SetRotation(m_turretRot);
		m_cannonRender->SetRotation(m_turretRot);

		m_baseRender->SetPosition(m_position);
		m_turretRender->SetPosition(m_position);
		m_cannonRender->SetPosition(m_position);

		if (m_hp <= 500.0f)
		{
			Vector2 screenPos;
			g_camera3D->CalcScreenPositionFromWorldPosition(screenPos, m_position);
			m_fontRender->SetPosition(screenPos);
		}
	}

	void BossTank::Damage(float damage)
	{
		m_hp -= damage;

		//体力半分で砲身切断可能に
		if (m_hp <= 500.0f)
		{
			if (m_fontRender == nullptr)
			{
				m_fontRender = NewGO<FontRender>(0);
				m_fontRender->SetText(CANNONCUT_TEXT);
				m_fontRender->SetShadowFlag(true);
				m_fontRender->SetShadowColor(SHADOWCOLOR_BLACK);
			}

			m_cannonRender->SetDivideFlag(true);
			ModelCutManager::GetInstance()->AddCuttable(m_cannonRender);
		}
	
		//体力0なら砲塔と本体も切断可能に
		if (m_hp < 0)
		{
			m_hp = 0;
			m_fontRender->SetText(ALLCUT_TEXT);
			m_baseRender->SetDivideFlag(true);
			m_turretRender->SetDivideFlag(true);
			ModelCutManager::GetInstance()->AddCuttable(m_baseRender);
			ModelCutManager::GetInstance()->AddCuttable(m_turretRender);
		}
	}
}