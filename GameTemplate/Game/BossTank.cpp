#include "stdafx.h"
#include "BossTank.h"
#include "EnemyMissile.h"

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

		m_hpRender = NewGO<FontRender>(1);
		wchar_t buffer[256];
		swprintf_s(buffer, L"BOSSHP:%d", m_hp);
		m_hpRender->SetText(buffer);
		m_hpRender->SetPosition({ 350.0f,-300.0f });
		m_hpRender->SetShadowFlag(true);
		m_hpRender->SetShadowColor(SHADOWCOLOR_BLACK);


		m_boxCollider.Create({ 400.0f,175.0f,800.0f });
		RigidBodyInitData rbInitData;
		rbInitData.mass = 100.0f;
		rbInitData.collider = &m_boxCollider;
		rbInitData.pos = m_position;
		rbInitData.rot = m_baseRot;

		rbInitData.localInteria.Set(
			0.0f,
			0.0f,
			0.0f
		);

		BossTankTasks bossTankTasks;
		bossTankTasks.SubmitTo(this);

		m_rigidBody.Init(rbInitData);
		m_rigidBody.SetFriction(10.0f);
		m_rigidBody.SetLinearFactor(0.0f, 0.0f, 0.0f);

		return true;
	}

	void BossTank::OnDivide(const SkinModelRender* skinModelRender)
	{
		//TODO:砲身が壊れたフラグ、行動を分岐させるのに使う
		m_isCannonBreak = true;
	}

	void BossTank::Update()
	{
		if (g_pad[0]->IsPress(enButtonLeft))
		{
			m_turretDeg += 1.0f;
		}

		if (g_pad[0]->IsPress(enButtonRight))
		{
			m_turretDeg -= 1.0f;
		}

		if (g_pad[0]->IsPress(enButtonUp))
		{
			m_baseDeg += 1.0f;
		}

		if (g_pad[0]->IsPress(enButtonDown))
		{
			m_baseDeg -= 1.0f;
		}

		if (g_pad[0]->IsTrigger(enButtonLB3))
		{
			//NOTE:タスクはコピーで登録している。コピーの是非は要検討
			m_taskQueue.push(m_tankTask[BossTankTasks::enMissile]);
			m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
			m_taskQueue.push(m_tankTask[BossTankTasks::enMissile]);
		}

		if (m_taskQueue.size() > 0)
		{
			bool isEnd = m_taskQueue.front().Execute();

			if (isEnd)
			{
				m_taskQueue.pop();
			}
		}

		if (g_pad[0]->IsTrigger(enButtonLB2))
		{
			while (m_taskQueue.size() != 0)
			{
				m_taskQueue.front().Terminate();
				m_taskQueue.pop();
			}
		}

		m_turretRot.SetRotationDegY(m_turretDeg);
		m_baseRot.SetRotationDegY(m_baseDeg);

		m_turretRot.Multiply(m_baseRot, m_turretRot);

		Vector3 rbPos = m_position;
		m_rigidBody.SetPositionAndRotation(m_position, m_baseRot);
		m_baseRender->SetRotation(m_baseRot);
		m_turretRender->SetRotation(m_turretRot);
		m_cannonRender->SetRotation(m_turretRot);

		m_baseRender->SetPosition(m_position);
		m_turretRender->SetPosition(m_position);
		m_cannonRender->SetPosition(m_position);
		m_cannonRender->SetToCoreVector({ 0.0f,175.0f,0.0f });

		wchar_t buffer[256];
		swprintf_s(buffer, L"BOSSHP:%d", m_hp);
		m_hpRender->SetText(buffer);

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
		if (m_hp <= 0)
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