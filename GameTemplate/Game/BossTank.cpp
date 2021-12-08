#include "stdafx.h"
#include "BossTank.h"
#include "EnemyMissile.h"
#include "Player.h"

namespace
{
	const char* MODEL_PATH_BASE = "Assets/modelData/TankBase.tkm";
	const char* MODEL_PATH_TURRET = "Assets/modelData/TankTurret.tkm";
	const char* MODEL_PATH_CANNON = "Assets/modelData/TankCannon.tkm";
	const wchar_t* CANNONCUT_TEXT = L"Can Cut Cannon";
	const wchar_t* ALLCUT_TEXT = L"Can Cut All";
	const Vector3 CANNON_VECTOR_TOCORE = { 0.0f,175.0f,0.0f };
	const float DISTANCE_RANGED_ATTACK = 1000.0f;
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
		m_bossTankCollision.Init(m_position, m_baseRot);

		//体力等の表示の初期化
		m_bossTankDisplay.Init(m_hp);

		//タスクの登録
		BossTankTasks bossTankTasks;
		bossTankTasks.SubmitTo(this);

		//出現時の待機タスク
		m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);

		return true;
	}

	void BossTank::OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce)
	{
		//斬られたのが砲身なら
		if (skinModelRender == m_cannonRender)
		{
			//砲身が壊れたフラグをオン、行動を分岐させるのに使う
			m_isCannonBreak = true;
		}

		//NOTE:baseRenderとturretRenderはおそらく同じくフレームで切断されるが、
		//どちらも切断された時のみDeleteする。

		//斬られたのが車体なら
		if (skinModelRender == m_baseRender)
		{
			//元々の車体のモデルをダミーとして生成
			m_baseRender->MakeDummy(cutForce);
			m_isBaseBreak = true;
		}

		//斬られたのが砲塔なら
		if (skinModelRender == m_turretRender)
		{
			//元々の砲塔のモデルをダミーとして作成
			m_turretRender->MakeDummy(cutForce);
			m_isTurretBreak = true;
		}

		//砲塔と本体がどちらも斬られたら完全にダミーに移行する
		if (m_isTurretBreak && m_isBaseBreak)
		{
			//残っていた砲身もダミーに
			m_cannonRender->MakeDummy(cutForce);

			//すべてダミーになったのでボスとしての挙動は不要なので削除
			DeleteGO(this);
		}
	}

	void BossTank::Update()
	{
		//ボスの行動パターン、別に分離したい
		if (m_taskQueue.size() == 0)
		{
			Player* player = FindGO<Player>("player");

			Vector3 distance = player->GetPosition() - m_position;

			//砲台を斬られてまだ雑魚敵を召喚していなかったら
			if (m_hp <= 500 && m_isSummonMinions == false && m_isCannonBreak == true)
			{
				m_taskQueue.push(m_tankTask[BossTankTasks::enSummon]);

				m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
				m_isSummonMinions = true;
			}
			else if (m_hp <= 500 && m_isCannonBreak == true)
			{
				//砲台が壊れた時
				if (distance.LengthSq() <= DISTANCE_RANGED_ATTACK * DISTANCE_RANGED_ATTACK)
				{
					//突進攻撃
					//(今はまだ回転攻撃)
					m_taskQueue.push(m_tankTask[BossTankTasks::enCharge]);
					m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
				}
				else
				{
					//岩攻撃
					m_taskQueue.push(m_tankTask[BossTankTasks::enRock]);
					m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
				}
			}
			else
			{
				//砲台が壊れていない時
				if (distance.LengthSq() <= DISTANCE_RANGED_ATTACK * DISTANCE_RANGED_ATTACK)
				{
					m_taskQueue.push(m_tankTask[BossTankTasks::enRolling]);
					m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
				}
				else
				{
					m_taskQueue.push(m_tankTask[BossTankTasks::enMissile]);
					m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
					m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
					m_taskQueue.push(m_tankTask[BossTankTasks::enWait]);
				}
			}
		}


		//キューにタスクがある場合
		if (m_taskQueue.size() > 0)
		{
			//先頭のタスクを実行してタスクが終わったかどうかを取得
			bool isEnd = m_taskQueue.front().Execute();


			//タスクが終わったら
			if (isEnd)
			{
				//終わったタスクをキューから削除
				m_taskQueue.pop();
			}
		}

		//デバッグ用、現在のタスクをリセットする
		if (g_pad[0]->IsTrigger(enButtonLB2))
		{
			while (m_taskQueue.size() != 0)
			{
				m_taskQueue.front().Terminate();
				m_taskQueue.pop();
			}
		}

		//砲塔の回転角度から回転を作成
		m_turretRot.SetRotationDegY(m_turretDeg);
		//車体の回転角度から回転を作成
		m_baseRot.SetRotationDegY(m_baseDeg);

		//砲塔の回転に車体の回転をかけ合わせる(車体の回転に砲塔が追従するため)
		m_turretRot.Multiply(m_baseRot, m_turretRot);

		//計算した回転をセット
		m_baseRender->SetRotation(m_baseRot);
		m_turretRender->SetRotation(m_turretRot);
		m_cannonRender->SetRotation(m_turretRot);

		//座標をセット
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
		//体力を減らす
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