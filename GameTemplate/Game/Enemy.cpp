#include "stdafx.h"
#include "Enemy.h"

namespace
{
	const char* PATH_ENEMYMODEL = "Assets/modelData/TwoMaterialOneMeshParts.tkm";
}

namespace Game
{
	Enemy::~Enemy()
	{
		if (m_enemyRender != nullptr)
		{
			DeleteGO(m_enemyRender);
		}
	}

	bool Enemy::Start()
	{
		m_enemyRender = NewGO<SkinModelRender>(0);
		m_enemyRender->Init(PATH_ENEMYMODEL);

		return true;
	}

	void Enemy::Update()
	{
		m_enemyRender->SetPosition(m_position);
	}

	void Enemy::Damage(float damage)
	{
		m_hp -= damage;

		if (m_hp <= 0)
		{
			m_hp = 0;

			m_enemyRender->SetDivideFlag(true);
			Game::ModelCutManager::GetInstance()->AddCuttable(m_enemyRender);
		}
	}
}