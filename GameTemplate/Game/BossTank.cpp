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
		//�ԑ́A�C���A�C�g��ʁX�̃��f���Ƃ��Đ���
		m_baseRender = NewGO<SkinModelRender>(0);
		m_turretRender = NewGO<SkinModelRender>(0);
		m_cannonRender = NewGO<SkinModelRender>(0);

		//������
		m_baseRender->Init(MODEL_PATH_BASE);
		m_turretRender->Init(MODEL_PATH_TURRET);
		m_cannonRender->Init(MODEL_PATH_CANNON);

		//�C�g�̃R�A(�ؒf���ꂽ�ꍇ�Ɏc�������ꏊ�̍��W)��ݒ�
		m_cannonRender->SetToCoreVector(CANNON_VECTOR_TOCORE);

		//���f���̏��L�҂�ݒ�
		m_baseRender->SetOwner(this);
		m_turretRender->SetOwner(this);
		m_cannonRender->SetOwner(this);

		//�����蔻��̏�����
		m_bossTankCollision.Init(m_bossTankStatus.position, m_bossTankStatus.baseRot);

		//�̗͓��̕\���̏�����
		m_bossTankDisplay.Init(m_bossTankStatus.hp);

		//�^�X�N�̓o�^
		m_bossTankBehave.SubmitTo(this);

		return true;
	}

	void BossTank::OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce)
	{
		//�a��ꂽ�̂��C�g�Ȃ�
		if (skinModelRender == m_cannonRender)
		{
			//�C�g����ꂽ�t���O���I���A�s���𕪊򂳂���̂Ɏg��
			m_bossTankStatus.isCannonBreak = true;
		}

		//NOTE:baseRender��turretRender�͂����炭�������t���[���Őؒf����邪�A
		//�ǂ�����ؒf���ꂽ���̂�Delete����B

		//�a��ꂽ�̂��ԑ̂Ȃ�
		if (skinModelRender == m_baseRender)
		{
			//���X�̎ԑ̂̃��f�����_�~�[�Ƃ��Đ���
			m_baseRender->MakeDummy(cutForce);
			m_bossTankStatus.isBaseBreak = true;
		}

		//�a��ꂽ�̂��C���Ȃ�
		if (skinModelRender == m_turretRender)
		{
			//���X�̖C���̃��f�����_�~�[�Ƃ��č쐬
			m_turretRender->MakeDummy(cutForce);
			m_bossTankStatus.isTurretBreak = true;
		}

		//�C���Ɩ{�̂��ǂ�����a��ꂽ�犮�S�Ƀ_�~�[�Ɉڍs����
		if (m_bossTankStatus.isTurretBreak && m_bossTankStatus.isBaseBreak)
		{
			//�c���Ă����C�g���_�~�[��
			m_cannonRender->MakeDummy(cutForce);

			//���ׂă_�~�[�ɂȂ����̂Ń{�X�Ƃ��Ă̋����͕s�v�Ȃ̂ō폜
			DeleteGO(this);
		}
	}

	void BossTank::Update()
	{
		//�s������
		m_bossTankBehave.Execute(m_bossTankStatus);

		//�C���̉�]�p�x�����]���쐬
		m_bossTankStatus.turretRot.SetRotationDegY(m_bossTankStatus.turretDeg);
		//�ԑ̂̉�]�p�x�����]���쐬
		m_bossTankStatus.baseRot.SetRotationDegY(m_bossTankStatus.baseDeg);

		//�C���̉�]�Ɏԑ̂̉�]���������킹��(�ԑ̂̉�]�ɖC�����Ǐ]���邽��)
		m_bossTankStatus.turretRot.Multiply(m_bossTankStatus.baseRot, m_bossTankStatus.turretRot);

		//�v�Z������]���Z�b�g
		m_baseRender->SetRotation(m_bossTankStatus.baseRot);
		m_turretRender->SetRotation(m_bossTankStatus.turretRot);
		m_cannonRender->SetRotation(m_bossTankStatus.turretRot);

		//���W���Z�b�g
		m_baseRender->SetPosition(m_bossTankStatus.position);
		m_turretRender->SetPosition(m_bossTankStatus.position);
		m_cannonRender->SetPosition(m_bossTankStatus.position);

		//�����蔻��̍X�V
		m_bossTankCollision.Update(m_bossTankStatus.position, m_bossTankStatus.baseRot);

		//�̗͓��̏��̍X�V
		m_bossTankDisplay.Update(m_bossTankStatus.hp, m_bossTankStatus.position);
	}

	void BossTank::Damage(float damage)
	{
		//�̗͂����炷
		m_bossTankStatus.hp -= damage;

		//�̗͔����ŖC�g�ؒf�\��
		if (m_bossTankStatus.hp <= 500.0f)
		{
			m_cannonRender->SetDivideFlag(true);
			ModelCutManager::GetInstance()->AddCuttable(m_cannonRender);
		}
	
		//�̗�0�Ȃ�C���Ɩ{�̂��ؒf�\��
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