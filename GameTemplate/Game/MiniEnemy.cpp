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
		//�ǂ����a���Ă��Ȃ������ꍇ�̂݃��f�����폜
		if (m_isDead == false)
		{
			DeleteGO(m_baseRender);
			DeleteGO(m_turretRender);
			DeleteGO(m_cannonRender);
		}
	}

	void MiniEnemy::OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce)
	{
		//�a��ꂽ�̂Ŏ��S
		m_isDead = true;

		//�a��ꂽ�̂��ԑ̂Ȃ�
		if (skinModelRender == m_baseRender)
		{
			//�ԑ̃��f������_�~�[����
			m_baseRender->MakeDummy(cutForce);
			m_turretRender->SetModelCenterAsOrigin();
			m_cannonRender->SetModelCenterAsOrigin();
			m_isBaseBreak = true;
		}

		//�a��ꂽ�̂��C���Ȃ�
		if (skinModelRender == m_turretRender)
		{
			//�C�����f������_�~�[����
			m_turretRender->MakeDummy(cutForce);
			m_baseRender->SetModelCenterAsOrigin();
			m_cannonRender->SetModelCenterAsOrigin();
			m_isTurretBreak = true;
		}

		//�a��ꂽ�̂��C�g�Ȃ�
		if (skinModelRender == m_cannonRender)
		{
			//�C�g���f������_�~�[����
			m_cannonRender->MakeDummy(cutForce);
			m_baseRender->SetModelCenterAsOrigin();
			m_turretRender->SetModelCenterAsOrigin();
			m_isCannonBreak = true;
		}

		//���ׂĂ̕��ʂ��a��ꂽ��
		if (m_isBaseBreak && m_isTurretBreak && m_isCannonBreak)
		{
			//���������̕K�v���Ȃ��̂ō폜
			DeleteGO(this);
		}
	}

	bool MiniEnemy::Start()
	{
		//�ԑ́A�C���A�C�g��ʁX�̃��f���Ƃ��Đ���
		m_baseRender = NewGO<SkinModelRender>(0);
		m_turretRender = NewGO<SkinModelRender>(0);
		m_cannonRender = NewGO<SkinModelRender>(0);

		//������
		m_baseRender->Init(MODEL_PATH_BASE);
		m_turretRender->Init(MODEL_PATH_TURRET);
		m_cannonRender->Init(MODEL_PATH_CANNON);
		
		m_baseRender->SetScale(ENEMY_SIZE);
		m_turretRender->SetScale(ENEMY_SIZE);
		m_cannonRender->SetScale(ENEMY_SIZE);

		//���f���̏��L�҂�ݒ�
		m_baseRender->SetOwner(this);
		m_turretRender->SetOwner(this);
		m_cannonRender->SetOwner(this);

		//�e���ʂ��ŏ�����a���悤�ɂ��Ă���
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
		//����ł����ꍇ�������Ȃ�
		if (m_isDead == true)
		{
			return;
		}

		//�ړ��J�n�t���[���Ȃ�
		if (m_actionFrame == FRMAE_MOVE_START)
		{
			//�v���C���[�ւ̌������v�Z
			Player* player = FindGO<Player>("player");
			Vector3 distance = player->GetPosition() - m_position;
			distance.Normalize();
			
			//�ړ������̓v���C���[�ւ̌���
			m_moveDirection = distance;

			//�v���C���[�Ɍ������Ďԑ̂�������
			m_baseRot.SetRotation(Vector3::Front, distance);
		}
		else if (m_actionFrame > FRMAE_MOVE_START && m_actionFrame < FRAME_ATTACK_START)
		{
			//�v���C���[�Ɍ������Ĉړ������
			m_position += m_moveDirection * CHARGE_SPEED;
		}
		else if (m_actionFrame > FRAME_ATTACK_START && m_actionFrame < FRAME_ATTACK_HIT)
		{
			//�C�g����
			m_turretDeg -= ROTATE_DEG;
		}
		else if (m_actionFrame == FRAME_ATTACK_HIT)
		{
			//�v���C���[�ɍU�����A�y��������΂�
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
			//�J�n�t���[���Ƀ��Z�b�g
			m_actionFrame = FRAME_ACTION_START;
		}

		//���W���Z�b�g
		m_baseRender->SetPosition(m_position);
		m_turretRender->SetPosition(m_position);
		m_cannonRender->SetPosition(m_position);

		//�C���̉�]���Z�b�g
		m_turretRot.SetRotationDegY(m_turretDeg);

		//�C���̉�]�Ɏԑ̂̉�]���������킹��
		m_turretRot.Multiply(m_baseRot, m_turretRot);

		//�v�Z������]���Z�b�g
		m_baseRender->SetRotation(m_baseRot);
		m_turretRender->SetRotation(m_turretRot);
		m_cannonRender->SetRotation(m_turretRot);

		//�o�߃t���[���𑝉�������
		m_actionFrame++;
	}
}