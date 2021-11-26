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
		if (m_isTurretBreak == false && m_isBaseBreak == false)
		{
			DeleteGO(m_baseRender);
			DeleteGO(m_turretRender);
			DeleteGO(m_cannonRender);
		}
	}

	bool BossTank::Start()
	{
		m_baseRender = NewGO<SkinModelRender>(0);
		m_turretRender = NewGO<SkinModelRender>(0);
		m_cannonRender = NewGO<SkinModelRender>(0);


		m_baseRender->Init(MODEL_PATH_BASE);
		m_turretRender->Init(MODEL_PATH_TURRET);
		m_cannonRender->Init(MODEL_PATH_CANNON);
		m_cannonRender->SetToCoreVector({ 0.0f,175.0f,0.0f });
		m_baseRender->SetOwner(this);
		m_turretRender->SetOwner(this);
		m_cannonRender->SetOwner(this);

		//当たり判定の初期化
		m_bossTankCollision.Init(m_position, m_baseRot);

		//体力等の表示の初期化
		m_bossTankDisplay.Init(m_hp);

		//タスクの登録
		BossTankTasks bossTankTasks;
		bossTankTasks.SubmitTo(this);

		return true;
	}

	void BossTank::OnDivide(const SkinModelRender* skinModelRender)
	{
		if (skinModelRender == m_cannonRender)
		{
			//砲身が壊れたフラグをオン、行動を分岐させるのに使う
			m_isCannonBreak = true;
		}

		//NOTE:baseRenderとturretRenderはおそらく同じくフレームで切断されるが、
		//どちらも切断された時のみDeleteする。
		if (skinModelRender == m_baseRender)
		{
			m_baseRender->MakeDummy();
			m_isBaseBreak = true;
		}

		if (skinModelRender == m_turretRender)
		{
			m_turretRender->MakeDummy();
			m_isTurretBreak = true;
		}

		//砲塔と本体がどちらも斬られたら完全にダミーに移行する
		if (m_isTurretBreak && m_isBaseBreak)
		{
			m_cannonRender->MakeDummy();
			DeleteGO(this);
		}
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

		m_baseRender->SetRotation(m_baseRot);
		m_turretRender->SetRotation(m_turretRot);
		m_cannonRender->SetRotation(m_turretRot);

		m_baseRender->SetPosition(m_position);
		m_turretRender->SetPosition(m_position);
		m_cannonRender->SetPosition(m_position);

		//当たり判定の更新
		m_bossTankCollision.Update(m_position, m_baseRot);

		//体力等の情報の更新
		m_bossTankDisplay.Update(m_hp, m_position);
	}

	void BossTank::Damage(float damage)
	{
		m_hp -= damage;

		//体力半分で砲身切断可能に
		if (m_hp <= 500.0f)
		{
			m_cannonRender->SetDivideFlag(true);
			ModelCutManager::GetInstance()->AddCuttable(m_cannonRender);
		}
	
		//体力0なら砲塔と本体も切断可能に
		if (m_hp <= 0)
		{
			m_hp = 0;
			m_baseRender->SetDivideFlag(true);
			m_turretRender->SetDivideFlag(true);
			ModelCutManager::GetInstance()->AddCuttable(m_baseRender);
			ModelCutManager::GetInstance()->AddCuttable(m_turretRender);
		}
	}
}