#include "stdafx.h"
#include "MiniEnemy.h"
#include "Player.h"

namespace
{
	const char* MODEL_PATH = "Assets/modelData/Drone.tkm";
	const int KNOCKDOWN_SPEED = 10;
	const int ATTACK_RANGE = 200;
	const int CHARGE_SPEED = 25;
	const float ROTATE_DEG = 15.0f;
	const int FRAME_ACTION_START = 0;
	const int FRMAE_MOVE_START = 60;
	const int FRAME_ATTACK_START = 80;
	const int FRAME_ATTACK_HIT = 105;
	const int FRAME_ACTION_END = 300;
	const int ATTACK_DAMAGE = 25;
	const Vector3 FLOATING_HEIGHT = { 0.0f,100.0f,0.0f };
}

namespace Game
{
	MiniEnemy::~MiniEnemy()
	{
		//�ǂ����a���Ă��Ȃ������ꍇ�̂݃��f�����폜
		if (m_isDead == false)
		{
			DeleteGO(m_baseRender);
		}
	}

	void MiniEnemy::OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce)
	{
		//�a��ꂽ�̂Ŏ��S
		m_isDead = true;

		//���f������_�~�[����
		m_baseRender->MakeDummy(cutForce);

		//���������̕K�v���Ȃ��̂ō폜
		DeleteGO(this);
	}

	bool MiniEnemy::Start()
	{
		//���f���̐���
		m_baseRender = NewGO<SkinModelRender>(0);

		//������
		m_baseRender->Init(MODEL_PATH);

		//���f���̏��L�҂�ݒ�
		m_baseRender->SetOwner(this);

		//�a��₷���悤�Ƀ��f���̃Z���^�[�����炷
		m_baseRender->SetModelCenterAsOrigin();

		//�ŏ�����a���悤�ɂ��Ă���
		m_baseRender->SetDivideFlag(true);
		ModelCutManager::GetInstance()->AddCuttable(m_baseRender);

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

			//�v���C���[�Ɍ���
			m_baseRot.SetRotation(Vector3::Front, distance);
		}
		else if (m_actionFrame > FRMAE_MOVE_START && m_actionFrame < FRAME_ATTACK_START)
		{
			//�v���C���[�Ɍ������Ĉړ������
			m_position += m_moveDirection * CHARGE_SPEED;
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
		m_baseRender->SetPosition(m_position + FLOATING_HEIGHT);

		//�v�Z������]���Z�b�g
		m_baseRender->SetRotation(m_baseRot);

		//�o�߃t���[���𑝉�������
		m_actionFrame++;
	}
}