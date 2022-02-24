#include "stdafx.h"
#include "MiniEnemy.h"
#include "Player.h"

namespace
{
	const char* MODEL_PATH = "Assets/modelData/Drone.tkm";						//�G���G�̃��f���p�X
	const int KNOCKDOWN_SPEED = 10;												//�G���G�̐�����΂����x
	const int ATTACK_RANGE = 200;												//�G���G�̍U���͈�
	const int CHARGE_SPEED = 25;												//�G���G�̈ړ����x
	const int FRAME_ACTION_START = 0;											//�s���J�n�t���[��
	const int FRMAE_MOVE_START = 60;											//�ړ��J�n�t���[��
	const int FRAME_ATTACK_START = 80;											//�U���J�n�t���[��
	const int FRAME_ATTACK_HIT = 105;											//�U���̓����蔻����쐬����t���[��
	const int FRAME_ACTION_END = 300;											//�s���I���t���[��
	const int ATTACK_DAMAGE = 25;												//�U���̃_���[�W
	const Vector3 FLOATING_HEIGHT = { 0.0f,100.0f,0.0f };						//���V���Ă��鍂���ւ̃x�N�g��
	const Vector3 ATTACK_HEIGHT = { 0.0f,50.0f,0.0f };							//�U���̒��S�n�_�ւ̃x�N�g��
	const char16_t* WARPEFFECT_PATH = u"Assets/effect/Teleport.efk";			//�o���G�t�F�N�g�p�X
	const char16_t* LIGHTNINGEFFECT_PATH = u"Assets/effect/ThunderAttack.efk";	//�d���U���G�t�F�N�g�p�X
	const wchar_t* LIGHTNING_SOUND_PATH = L"Assets/sound/LightningSE.wav";		//�d���U���̌��ʉ��p�X
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
		m_baseRender = NewGO<SkinModelRender>(Priority::High);

		//������
		m_baseRender->Init(MODEL_PATH);

		//���f���̏��L�҂�ݒ�
		m_baseRender->SetOwner(this);

		//�a��₷���悤�Ƀ��f���̃Z���^�[�����炷
		m_baseRender->SetModelCenterAsOrigin();

		//�ŏ�����a���悤�ɂ��Ă���
		m_baseRender->SetDivideFlag(true);
		ModelCutManager::GetInstance()->AddCuttable(m_baseRender);

		//�o���G�t�F�N�g���Đ�
		Game::Effect* spawnEffect = NewGO<Game::Effect>(Priority::High);
		spawnEffect->SetPosition(m_position);
		spawnEffect->Init(WARPEFFECT_PATH);
		spawnEffect->Play();

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
		//�ړ����Ȃ�
		else if (m_actionFrame > FRMAE_MOVE_START && m_actionFrame < FRAME_ATTACK_START)
		{
			//�v���C���[�Ɍ������Ĉړ������
			m_position += m_moveDirection * CHARGE_SPEED;
		}
		//�U���̓����蔻��t���[���Ȃ�
		else if (m_actionFrame == FRAME_ATTACK_HIT)
		{
			//�v���C���[�ɍU�����A�y��������΂�
			Player* player = FindGO<Player>("player");
			Vector3 distance = player->GetPosition() - m_position;

			//�d���G�t�F�N�g���Đ�
			Game::Effect* attackEffect = NewGO<Game::Effect>(Priority::High);
			attackEffect->Init(LIGHTNINGEFFECT_PATH);
			attackEffect->SetPosition(m_position + ATTACK_HEIGHT);
			attackEffect->Play();

			//�d�����ʉ����Đ�
			SoundOneShotPlay(LIGHTNING_SOUND_PATH);

			//�U���͈͓��Ȃ�v���C���[�Ƀ_���[�W��^���Đ�����΂�
			if (distance.LengthSq() < ATTACK_RANGE * ATTACK_RANGE)
			{
				distance.Normalize();
				player->KnockDown(distance * KNOCKDOWN_SPEED);
				player->Damage(ATTACK_DAMAGE);
			}
		}
		//�s���I���t���[���Ȃ�
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