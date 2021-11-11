#include "stdafx.h"
#include "Enemy.h"

namespace
{
	const char* PATH_ENEMYMODEL = "Assets/modelData/TwoMaterialOneMeshParts.tkm";
	const wchar_t* CANCUT_TEXT = L"Can Cut";
	const Vector4 SHADOWCOLOR_BLACK = { 0.0f,0.0f,0.0f,1.0f };
}

namespace Game
{
	Enemy::~Enemy()
	{
		if (m_enemyRender != nullptr)
		{
			DeleteGO(m_enemyRender);
		}

		if (m_canCutTextRender != nullptr)
		{
			DeleteGO(m_canCutTextRender);
		}
	}

	bool Enemy::Start()
	{
		m_enemyRender = NewGO<SkinModelRender>(0);
		m_enemyRender->Init(PATH_ENEMYMODEL);
		m_enemyRender->SetOwner(this);

		return true;
	}

	void Enemy::Update()
	{
		m_position.x += 1.0f;
		m_enemyRender->SetPosition(m_position);
		m_enemyRender->SetRotation(m_qRot);

		if (m_hp <= 0)
		{
			Vector2 screenPos;
			g_camera3D->CalcScreenPositionFromWorldPosition(screenPos, m_position);

			m_canCutTextRender->SetPosition(screenPos);
		}
	}

	void Enemy::OnDivide()
	{
		OutputDebugStringA("OnDivide");
	}

	void Enemy::Damage(float damage)
	{
		if (m_hp > 0)
		{
			m_hp -= damage;

			if (m_hp <= 0)
			{
				m_hp = 0;

				m_enemyRender->SetDivideFlag(true);
				Game::ModelCutManager::GetInstance()->AddCuttable(m_enemyRender);

				m_canCutTextRender = NewGO<FontRender>(0);
				m_canCutTextRender->SetText(CANCUT_TEXT);
				m_canCutTextRender->SetShadowFlag(true);
				m_canCutTextRender->SetShadowColor(SHADOWCOLOR_BLACK);
			}
		}
	}
}