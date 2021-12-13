#include "stdafx.h"
#include "BossTank.h"

namespace
{
	const char* MODEL_PATH_BASE = "Assets/modelData/TankBase.tkm";
	const char* MODEL_PATH_TURRET = "Assets/modelData/TankTurret.tkm";
	const char* MODEL_PATH_CANNON = "Assets/modelData/TankCannon.tkm";
	const Vector3 CANNON_VECTOR_TOCORE = { 0.0f,175.0f,0.0f };
}

namespace Game
{
	BossTank::~BossTank()
	{
		if (m_bossTankStatus.isTurretBreak == false && m_bossTankStatus.isBaseBreak == false)
		{
			DeleteGO(m_baseRender);
			DeleteGO(m_turretRender);
			DeleteGO(m_cannonRender);
		}
	}

	bool BossTank::Start()
	{
		//車体、砲塔、砲身を別々のモデルとして生成
		m_baseRender = NewGO<SkinModelRender>(0);
		m_turretRender = NewGO<SkinModelRender>(0);
		m_cannonRender = NewGO<SkinModelRender>(0);

		//初期化
		m_baseRender->Init(MODEL_PATH_BASE);
		m_turretRender->Init(MODEL_PATH_TURRET);
		m_cannonRender->Init(MODEL_PATH_CANNON);

		//砲身のコア(切断された場合に残したい場所の座標)を設定
		m_cannonRender->SetToCoreVector(CANNON_VECTOR_TOCORE);

		//モデルの所有者を設定
		m_baseRender->SetOwner(this);
		m_turretRender->SetOwner(this);
		m_cannonRender->SetOwner(this);

		//当たり判定の初期化
		m_bossTankCollision.Init(m_bossTankStatus.position, m_bossTankStatus.baseRot);

		//体力等の表示の初期化
		m_bossTankDisplay.Init(m_bossTankStatus.hp);

		//タスクの登録
		m_bossTankBehave.SubmitTo(this);

		return true;
	}

	void BossTank::OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce)
	{
		//斬られたのが砲身なら
		if (skinModelRender == m_cannonRender)
		{
			//砲身が壊れたフラグをオン、行動を分岐させるのに使う
			m_bossTankStatus.isCannonBreak = true;
		}

		//NOTE:baseRenderとturretRenderはおそらく同じくフレームで切断されるが、
		//どちらも切断された時のみDeleteする。

		//斬られたのが車体なら
		if (skinModelRender == m_baseRender)
		{
			//元々の車体のモデルをダミーとして生成
			m_baseRender->MakeDummy(cutForce);
			m_bossTankStatus.isBaseBreak = true;
		}

		//斬られたのが砲塔なら
		if (skinModelRender == m_turretRender)
		{
			//元々の砲塔のモデルをダミーとして作成
			m_turretRender->MakeDummy(cutForce);
			m_bossTankStatus.isTurretBreak = true;
		}

		//砲塔と本体がどちらも斬られたら完全にダミーに移行する
		if (m_bossTankStatus.isTurretBreak && m_bossTankStatus.isBaseBreak)
		{
			//残っていた砲身もダミーに
			m_cannonRender->MakeDummy(cutForce);

			//すべてダミーになったのでボスとしての挙動は不要なので削除
			DeleteGO(this);
		}
	}

	void BossTank::Update()
	{
		//行動分岐
		m_bossTankBehave.Execute(m_bossTankStatus);

		//砲塔の回転角度から回転を作成
		m_bossTankStatus.turretRot.SetRotationDegY(m_bossTankStatus.turretDeg);
		//車体の回転角度から回転を作成
		m_bossTankStatus.baseRot.SetRotationDegY(m_bossTankStatus.baseDeg);

		//砲塔の回転に車体の回転をかけ合わせる(車体の回転に砲塔が追従するため)
		m_bossTankStatus.turretRot.Multiply(m_bossTankStatus.baseRot, m_bossTankStatus.turretRot);

		//計算した回転をセット
		m_baseRender->SetRotation(m_bossTankStatus.baseRot);
		m_turretRender->SetRotation(m_bossTankStatus.turretRot);
		m_cannonRender->SetRotation(m_bossTankStatus.turretRot);

		//座標をセット
		m_baseRender->SetPosition(m_bossTankStatus.position);
		m_turretRender->SetPosition(m_bossTankStatus.position);
		m_cannonRender->SetPosition(m_bossTankStatus.position);

		//当たり判定の更新
		m_bossTankCollision.Update(m_bossTankStatus.position, m_bossTankStatus.baseRot);

		//体力等の情報の更新
		m_bossTankDisplay.Update(m_bossTankStatus.hp, m_bossTankStatus.position);
	}

	void BossTank::Damage(float damage)
	{
		//体力を減らす
		m_bossTankStatus.hp -= damage;

		//体力半分で砲身切断可能に
		if (m_bossTankStatus.hp <= 500.0f)
		{
			m_cannonRender->SetDivideFlag(true);
			ModelCutManager::GetInstance()->AddCuttable(m_cannonRender);
		}
	
		//体力0なら砲塔と本体も切断可能に
		if (m_bossTankStatus.hp <= 0)
		{
			m_bossTankStatus.hp = 0;
			m_baseRender->SetDivideFlag(true);
			m_turretRender->SetDivideFlag(true);
			ModelCutManager::GetInstance()->AddCuttable(m_baseRender);
			ModelCutManager::GetInstance()->AddCuttable(m_turretRender);
		}
	}
}