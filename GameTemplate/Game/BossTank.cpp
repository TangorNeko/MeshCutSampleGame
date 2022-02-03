#include "stdafx.h"
#include "BossTank.h"
#include "Player.h"
#include "Explosion.h"

namespace
{
	const char* MODEL_PATH_BASE = "Assets/modelData/TankBase.tkm";
	const char* MODEL_PATH_TURRET = "Assets/modelData/TankTurret.tkm";
	const char* MODEL_PATH_CANNONRIGHT = "Assets/modelData/TankCannonRight.tkm";
	const char* MODEL_PATH_CANNONLEFT = "Assets/modelData/TankCannonLeft.tkm";
	const Vector3 CANNON_VECTOR_TOCORE = { 0.0f,175.0f,-100.0f };
	const int MAX_HP = 400;
	const char16_t* WARPEFFECT_PATH = u"Assets/effect/Teleport.efk";
	const Vector3 TO_RIGHTCANNON_VECTOR = { 160.0f,250.0f,0.0f };
	const Vector3 TO_LEFTCANNON_VECTOR = { -160.0f,250.0f,0.0f };
	const Vector3 TO_FRONT_VECTOR = { 0.0f, 190.0f, 300.0f };
	const float PLAYER_BACKHANDSPRING_POWER = 5.0f;
}

namespace Game
{
	BossTank::~BossTank()
	{
		if (m_bossTankStatus.isTurretBreak == false && m_bossTankStatus.isBaseBreak == false)
		{
			DeleteGO(m_baseRender);
			DeleteGO(m_turretRender);
			DeleteGO(m_rightCannonRender);
			DeleteGO(m_leftCannonRender);
		}
	}

	bool BossTank::Start()
	{
		//�ԑ́A�C���A�C�g��ʁX�̃��f���Ƃ��Đ���
		m_baseRender = NewGO<SkinModelRender>(0);
		m_turretRender = NewGO<SkinModelRender>(0);
		m_rightCannonRender = NewGO<SkinModelRender>(0);
		m_leftCannonRender = NewGO<SkinModelRender>(0);

		//������
		m_baseRender->Init(MODEL_PATH_BASE);
		m_turretRender->Init(MODEL_PATH_TURRET);
		m_rightCannonRender->Init(MODEL_PATH_CANNONRIGHT);
		m_leftCannonRender->Init(MODEL_PATH_CANNONLEFT);

		//�C�g�̃R�A(�ؒf���ꂽ�ꍇ�Ɏc�������ꏊ�̍��W)��ݒ�
		m_rightCannonRender->SetToCoreVector(CANNON_VECTOR_TOCORE);
		m_leftCannonRender->SetToCoreVector(CANNON_VECTOR_TOCORE);

		//���f���̏��L�҂�ݒ�
		m_baseRender->SetOwner(this);
		m_turretRender->SetOwner(this);
		m_rightCannonRender->SetOwner(this);
		m_leftCannonRender->SetOwner(this);

		//�����蔻��̏�����
		m_bossTankCollision.Init(m_bossTankStatus.position, m_bossTankStatus.baseRot);

		//�̗͓��̕\���̏�����
		m_bossTankDisplay.Init(m_bossTankStatus.hp);

		//�^�X�N�̓o�^
		m_bossTankBehave.SubmitTo(this);

		//�o���G�t�F�N�g���Đ�
		Game::Effect* spawnEffect = NewGO<Game::Effect>(1);
		spawnEffect->SetPosition(m_bossTankStatus.position);
		spawnEffect->Init(WARPEFFECT_PATH);
		spawnEffect->Play();

		return true;
	}

	void BossTank::OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce)
	{
		Player* player = FindGO<Player>("player");

		//�a��ꂽ�̂��C�g�Ȃ�
		if ((skinModelRender == m_rightCannonRender || skinModelRender == m_leftCannonRender) && m_bossTankStatus.isCannonBreak == false)
		{
			//�C�g����ꂽ�t���O���I���A�s���𕪊򂳂���̂Ɏg��
			m_bossTankStatus.isCannonBreak = true;

			m_bossTankBehave.TerminateTask();

			player->NoticeMissileMoveEnd();
		}

		//NOTE:baseRender��turretRender�͂����炭�������t���[���Őؒf����邪�A
		//�ǂ�����ؒf���ꂽ���̂�Delete����B

		//�a��ꂽ�̂��ԑ̂Ȃ�
		if (skinModelRender == m_baseRender)
		{
			//���X�̎ԑ̂̃��f�����_�~�[�Ƃ��Đ���
			m_baseRender->MakeDummy(cutForce);
			player->NoticeFrontMoveEnd();
			m_bossTankStatus.isBaseBreak = true;
		}

		//�a��ꂽ�̂��C���Ȃ�
		if (skinModelRender == m_turretRender)
		{
			//���X�̖C���̃��f�����_�~�[�Ƃ��č쐬
			m_turretRender->MakeDummy(cutForce);
			player->NoticeFrontMoveEnd();
			m_bossTankStatus.isTurretBreak = true;
		}

		//�C���Ɩ{�̂��ǂ�����a��ꂽ�犮�S�Ƀ_�~�[�Ɉڍs����
		if (m_bossTankStatus.isTurretBreak && m_bossTankStatus.isBaseBreak)
		{
			//�c���Ă����C�g���_�~�[��
			m_rightCannonRender->MakeDummy(cutForce);
			m_leftCannonRender->MakeDummy(cutForce);

			Vector3 toResPos = player->GetPosition() - m_bossTankStatus.position;

			toResPos.y = 0.0f;
			toResPos.Normalize();

			player->BackHandSpring(toResPos * PLAYER_BACKHANDSPRING_POWER);
			player->StartFinishCamera();

			Explosion* explosion = NewGO<Explosion>(0);
			explosion->SetPosition(m_bossTankStatus.position);

			//���ׂă_�~�[�ɂȂ����̂Ń{�X�Ƃ��Ă̋����͕s�v�Ȃ̂ō폜
			DeleteGO(this);
		}
	}

	void BossTank::Update()
	{
		//�g�h���p�̃R�}���h��\�����邩�̃`�F�b�N
		m_bossTankFinishCommand.Execute(m_bossTankStatus.position, m_bossTankStatus.hp);

		//�s������
		m_bossTankBehave.Execute(m_bossTankStatus);

		//�C���̉�]�p�x�����]���쐬
		m_bossTankStatus.turretRot.SetRotationDegY(m_bossTankStatus.turretDeg);
		//�ԑ̂̉�]�p�x�����]���쐬
		m_bossTankStatus.baseRot.SetRotationDegY(m_bossTankStatus.baseDeg);

		//�C���̉�]�Ɏԑ̂̉�]���������킹��(�ԑ̂̉�]�ɖC�����Ǐ]���邽��)
		m_bossTankStatus.turretRot.Multiply(m_bossTankStatus.baseRot, m_bossTankStatus.turretRot);


		if (g_pad[0]->IsPress(enButtonY))
		{
			Player* player = FindGO<Player>("player");
			Vector3 toPlayer = player->GetPosition() - m_bossTankStatus.position;

			toPlayer.y = 0.0f;
			toPlayer.Normalize();

			m_bossTankStatus.trackingRot.SetRotation(Vector3::Front, toPlayer);

		}
		
		m_bossTankStatus.turretRot.Multiply(m_bossTankStatus.trackingRot);


		//�v�Z������]���Z�b�g
		m_baseRender->SetRotation(m_bossTankStatus.baseRot);
		m_turretRender->SetRotation(m_bossTankStatus.turretRot);
		m_rightCannonRender->SetRotation(m_bossTankStatus.turretRot);
		m_leftCannonRender->SetRotation(m_bossTankStatus.turretRot);

		//���W���Z�b�g
		m_baseRender->SetPosition(m_bossTankStatus.position);
		m_turretRender->SetPosition(m_bossTankStatus.position);
		m_rightCannonRender->SetPosition(m_bossTankStatus.position);
		m_leftCannonRender->SetPosition(m_bossTankStatus.position);

		//�����蔻��̍X�V
		m_bossTankCollision.Update(m_bossTankStatus.position, m_bossTankStatus.baseRot);

		//�̗͓��̏��̍X�V
		m_bossTankDisplay.Update(m_bossTankStatus.hp, m_bossTankStatus.position);

		//�a��ꂽ�̂��ԑ̂Ȃ�
		if (m_bossTankStatus.isBaseBreak == true && m_bossTankStatus.isTurretBreak == false)
		{
			//���X�̖C���̃��f�����_�~�[�Ƃ��č쐬
			m_turretRender->MakeDummy(Vector3::Zero);
			m_bossTankStatus.isTurretBreak = true;

			//�c���Ă����C�g���_�~�[��
			m_rightCannonRender->MakeDummy(Vector3::Zero);
			m_leftCannonRender->MakeDummy(Vector3::Zero);

			//���ׂă_�~�[�ɂȂ����̂Ń{�X�Ƃ��Ă̋����͕s�v�Ȃ̂ō폜
			DeleteGO(this);
		}

		//�a��ꂽ�̂��C���Ȃ�
		if (m_bossTankStatus.isTurretBreak == true && m_bossTankStatus.isBaseBreak == false)
		{
			//���X�̎ԑ̂̃��f�����_�~�[�Ƃ��Đ���
			m_baseRender->MakeDummy(Vector3::Zero);
			m_bossTankStatus.isBaseBreak = true;

			//�c���Ă����C�g���_�~�[��
			m_rightCannonRender->MakeDummy(Vector3::Zero);
			m_leftCannonRender->MakeDummy(Vector3::Zero);

			//���ׂă_�~�[�ɂȂ����̂Ń{�X�Ƃ��Ă̋����͕s�v�Ȃ̂ō폜
			DeleteGO(this);
		}
	}

	void BossTank::Damage(float damage)
	{
		//�̗͂����炷
		m_bossTankStatus.hp -= damage;

		//�̗͔����ŖC�g�ؒf�\��
		if (m_bossTankStatus.hp <= MAX_HP/2)
		{
			m_rightCannonRender->SetDivideFlag(true);
			m_leftCannonRender->SetDivideFlag(true);
		}
	
		//�̗�0�Ȃ�C���Ɩ{�̂��ؒf�\��
		if (m_bossTankStatus.hp <= 0)
		{
			m_bossTankStatus.hp = 0;
			m_baseRender->SetDivideFlag(true);
			m_turretRender->SetDivideFlag(true);
			m_bossTankBehave.TerminateTask();
		}
	}

	Vector3 BossTank::GetRightCannonPosition()
	{
		Vector3 toRightCannonVec = TO_RIGHTCANNON_VECTOR;

		m_bossTankStatus.turretRot.Apply(toRightCannonVec);

		return m_bossTankStatus.position + toRightCannonVec;
	}

	Vector3 BossTank::GetLeftCannonPosition()
	{
		Vector3 toLeftCannonVec = TO_LEFTCANNON_VECTOR;

		m_bossTankStatus.turretRot.Apply(toLeftCannonVec);

		return m_bossTankStatus.position + toLeftCannonVec;
	}

	Vector3 BossTank::GetFrontPosition()
	{
		Vector3 toFrontVec = TO_FRONT_VECTOR;

		m_bossTankStatus.baseRot.Apply(toFrontVec);

		return m_bossTankStatus.position + toFrontVec;
	}

	void BossTank::AllowCannonCut()
	{
		ModelCutManager::GetInstance()->AddCuttable(m_rightCannonRender);
		ModelCutManager::GetInstance()->AddCuttable(m_leftCannonRender);
	}

	void BossTank::AllowBodyCut()
	{
		ModelCutManager::GetInstance()->AddCuttable(m_baseRender);
		ModelCutManager::GetInstance()->AddCuttable(m_turretRender);
	}
}