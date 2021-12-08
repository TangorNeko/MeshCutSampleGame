#include "stdafx.h"
#include "MiniEnemy.h"
#include "Player.h"

namespace
{
	const char* MODEL_PATH_BASE = "Assets/modelData/TankBase.tkm";
	const char* MODEL_PATH_TURRET = "Assets/modelData/TankTurret.tkm";
	const char* MODEL_PATH_CANNON = "Assets/modelData/TankCannon.tkm";
	Vector3 ENEMY_SIZE = { 0.3f,0.3f,0.3f };
	int KNOCKDOWN_SPEED = 10;
	int ATTACK_RANGE = 200;
	int CHARGE_SPEED = 25;
	float ROTATE_DEG = 15.0f;
	int FRAME_ACTION_START = 0;
	int FRMAE_MOVE_START = 60;
	int FRAME_ATTACK_START = 80;
	int FRAME_ATTACK_HIT = 105;
	int FRAME_ACTION_END = 300;
	int ATTACK_DAMAGE = 25;
}

namespace Game
{
	MiniEnemy::~MiniEnemy()
	{
		//どこも斬られていなかった場合のみモデルを削除
		if (m_isDead == false)
		{
			DeleteGO(m_baseRender);
			DeleteGO(m_turretRender);
			DeleteGO(m_cannonRender);
		}
	}

	void MiniEnemy::OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce)
	{
		//斬られたので死亡
		m_isDead = true;

		//斬られたのが車体なら
		if (skinModelRender == m_baseRender)
		{
			//車体モデルからダミー生成
			m_baseRender->MakeDummy(cutForce);
			m_turretRender->SetModelCenterAsOrigin();
			m_cannonRender->SetModelCenterAsOrigin();
			m_isBaseBreak = true;
		}

		//斬られたのが砲塔なら
		if (skinModelRender == m_turretRender)
		{
			//砲塔モデルからダミー生成
			m_turretRender->MakeDummy(cutForce);
			m_baseRender->SetModelCenterAsOrigin();
			m_cannonRender->SetModelCenterAsOrigin();
			m_isTurretBreak = true;
		}

		//斬られたのが砲身なら
		if (skinModelRender == m_cannonRender)
		{
			//砲身モデルからダミー生成
			m_cannonRender->MakeDummy(cutForce);
			m_baseRender->SetModelCenterAsOrigin();
			m_turretRender->SetModelCenterAsOrigin();
			m_isCannonBreak = true;
		}

		//すべての部位が斬られたら
		if (m_isBaseBreak && m_isTurretBreak && m_isCannonBreak)
		{
			//もう処理の必要がないので削除
			DeleteGO(this);
		}
	}

	bool MiniEnemy::Start()
	{
		//車体、砲塔、砲身を別々のモデルとして生成
		m_baseRender = NewGO<SkinModelRender>(0);
		m_turretRender = NewGO<SkinModelRender>(0);
		m_cannonRender = NewGO<SkinModelRender>(0);

		//初期化
		m_baseRender->Init(MODEL_PATH_BASE);
		m_turretRender->Init(MODEL_PATH_TURRET);
		m_cannonRender->Init(MODEL_PATH_CANNON);
		
		m_baseRender->SetScale(ENEMY_SIZE);
		m_turretRender->SetScale(ENEMY_SIZE);
		m_cannonRender->SetScale(ENEMY_SIZE);

		//モデルの所有者を設定
		m_baseRender->SetOwner(this);
		m_turretRender->SetOwner(this);
		m_cannonRender->SetOwner(this);

		//各部位を最初から斬れるようにしておく
		m_baseRender->SetDivideFlag(true);
		m_turretRender->SetDivideFlag(true);
		m_cannonRender->SetDivideFlag(true);
		ModelCutManager::GetInstance()->AddCuttable(m_baseRender);
		ModelCutManager::GetInstance()->AddCuttable(m_turretRender);
		ModelCutManager::GetInstance()->AddCuttable(m_cannonRender);

		return true;
	}

	void MiniEnemy::Update()
	{
		//死んでいた場合何もしない
		if (m_isDead == true)
		{
			return;
		}

		//移動開始フレームなら
		if (m_actionFrame == FRMAE_MOVE_START)
		{
			//プレイヤーへの向きを計算
			Player* player = FindGO<Player>("player");
			Vector3 distance = player->GetPosition() - m_position;
			distance.Normalize();
			
			//移動方向はプレイヤーへの向き
			m_moveDirection = distance;

			//プレイヤーに向かって車体を向ける
			m_baseRot.SetRotation(Vector3::Front, distance);
		}
		else if (m_actionFrame > FRMAE_MOVE_START && m_actionFrame < FRAME_ATTACK_START)
		{
			//プレイヤーに向かって移動するる
			m_position += m_moveDirection * CHARGE_SPEED;
		}
		else if (m_actionFrame > FRAME_ATTACK_START && m_actionFrame < FRAME_ATTACK_HIT)
		{
			//砲身を回す
			m_turretDeg -= ROTATE_DEG;
		}
		else if (m_actionFrame == FRAME_ATTACK_HIT)
		{
			//プレイヤーに攻撃し、軽く吹き飛ばす
			Player* player = FindGO<Player>("player");
			Vector3 distance = player->GetPosition() - m_position;

			if (distance.LengthSq() < ATTACK_RANGE * ATTACK_RANGE)
			{
				distance.Normalize();
				player->KnockDown(distance * KNOCKDOWN_SPEED);
				player->Damage(ATTACK_DAMAGE);
			}
		}
		else if (m_actionFrame == FRAME_ACTION_END)
		{
			//開始フレームにリセット
			m_actionFrame = FRAME_ACTION_START;
		}

		//座標をセット
		m_baseRender->SetPosition(m_position);
		m_turretRender->SetPosition(m_position);
		m_cannonRender->SetPosition(m_position);

		//砲塔の回転をセット
		m_turretRot.SetRotationDegY(m_turretDeg);

		//砲塔の回転に車体の回転をかけ合わせる
		m_turretRot.Multiply(m_baseRot, m_turretRot);

		//計算した回転をセット
		m_baseRender->SetRotation(m_baseRot);
		m_turretRender->SetRotation(m_turretRot);
		m_cannonRender->SetRotation(m_turretRot);

		//経過フレームを増加させる
		m_actionFrame++;
	}
}