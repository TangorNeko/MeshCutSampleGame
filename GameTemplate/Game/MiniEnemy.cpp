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
}

namespace Game
{
	MiniEnemy::~MiniEnemy()
	{
		if (m_isDead == false)
		{
			DeleteGO(m_baseRender);
			DeleteGO(m_turretRender);
			DeleteGO(m_cannonRender);
		}
	}

	void MiniEnemy::OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce)
	{
		m_isDead = true;

		m_baseRender->MakeDummy(cutForce);
		m_turretRender->MakeDummy(cutForce);
		m_cannonRender->MakeDummy(cutForce);

		DeleteGO(this);
	}

	bool MiniEnemy::Start()
	{
		m_baseRender = NewGO<SkinModelRender>(0);
		m_turretRender = NewGO<SkinModelRender>(0);
		m_cannonRender = NewGO<SkinModelRender>(0);

		m_baseRender->Init(MODEL_PATH_BASE);
		m_turretRender->Init(MODEL_PATH_TURRET);
		m_cannonRender->Init(MODEL_PATH_CANNON);
		
		m_baseRender->SetScale(ENEMY_SIZE);
		m_turretRender->SetScale(ENEMY_SIZE);
		m_cannonRender->SetScale(ENEMY_SIZE);

		m_baseRender->SetOwner(this);
		m_turretRender->SetOwner(this);
		m_cannonRender->SetOwner(this);

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
		if (m_isDead == true)
		{
			return;
		}

		m_frame++;
		if (m_frame == FRMAE_MOVE_START)
		{
			Player* player = FindGO<Player>("player");
			Vector3 distance = player->GetPosition() - m_position;
			distance.Normalize();

			move = distance;

			rotation.SetRotation(Vector3::Front, distance);
		}
		else if (m_frame > FRMAE_MOVE_START && m_frame < FRAME_ATTACK_START)
		{
			m_position += move * CHARGE_SPEED;
		}
		else if (m_frame > FRAME_ATTACK_START && m_frame < FRAME_ATTACK_HIT)
		{
			m_turretDeg -= ROTATE_DEG;
		}
		else if (m_frame == FRAME_ATTACK_HIT)
		{
			Player* player = FindGO<Player>("player");
			Vector3 distance = player->GetPosition() - m_position;

			if (distance.LengthSq() < ATTACK_RANGE * ATTACK_RANGE)
			{
				distance.Normalize();
				player->KnockDown(distance * KNOCKDOWN_SPEED);
			}
		}
		else if (m_frame == FRAME_ACTION_END)
		{
			m_frame = FRAME_ACTION_START;
		}

		m_baseRender->SetPosition(m_position);
		m_turretRender->SetPosition(m_position);
		m_cannonRender->SetPosition(m_position);

		Quaternion Rot = rotation;
		Quaternion turretRot = Quaternion::Identity;

		turretRot.SetRotationDegY(m_turretDeg);

		turretRot.Multiply(Rot, turretRot);

		m_baseRender->SetRotation(rotation);
		m_turretRender->SetRotation(turretRot);
		m_cannonRender->SetRotation(turretRot);
	}
}