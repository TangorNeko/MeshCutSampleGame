#include "stdafx.h"
#include "CutDummy.h"

namespace
{
	const int TIME_DITHER_START = 50;
	const int FRAME_DELETE = 360;
	const float DUMMY_CAPSULE_COLLISION_DIVIDENUM = 3.0f;
	const float DUMMY_RIGIDBODY_MASS = 100.0f;
	const float DUMMY_RIGIDBODY_LOCALINTERIA = 0.1f;
	const float DUMMY_RIGIDBODY_FRICTION = 10.0f;
	const float DUMMY_RIGIDBODY_LINEARFACTOR = 1.0f;
}

namespace Game
{
	CutDummy::~CutDummy()
	{
		//�J�b�g����Ă��Ȃ������烂�f�����폜
		if (m_isCut == false)
		{
			DeleteGO(m_dummyModel);
		}

		//���̂͏�ɍ폜
		delete m_rigidBody;
	}

	bool CutDummy::Start()
	{
		m_rigidBody = new RigidBody;

		//�J�v�Z����̃R���C�_�[���쐬
		m_capsuleCollider.Init(m_dummyRadius/ DUMMY_CAPSULE_COLLISION_DIVIDENUM,m_dummyHeight/ DUMMY_CAPSULE_COLLISION_DIVIDENUM);
		
		//����̃R���C�_�[���쐬
		RigidBodyInitData rbInitData;

		//�d�ʂ��Z�b�g(�T���v���̂܂�)
		rbInitData.mass = DUMMY_RIGIDBODY_MASS;

		//�R���C�_�[���Z�b�g
		rbInitData.collider = &m_capsuleCollider;

		//���f���̉�]��ۑ����Ă���
		m_modelRot = m_dummyModel->GetRotation();

		//�������W�Ɖ�]���Z�b�g
		rbInitData.pos = m_dummyModel->GetPosition();
		rbInitData.rot = m_capsuleRot;

		//��]�̂��₷����ݒ肷��B(0�`1�A�T���v���̂܂�)
		rbInitData.localInteria.Set(
			DUMMY_RIGIDBODY_LOCALINTERIA,
			DUMMY_RIGIDBODY_LOCALINTERIA,
			DUMMY_RIGIDBODY_LOCALINTERIA
		);

		m_rigidBody->Init(rbInitData);
		//���C�͂�ݒ肷��B(0�`10�A�T���v���̂܂�)
		m_rigidBody->SetFriction(DUMMY_RIGIDBODY_FRICTION);
		//���`�ړ�����v�f��ݒ肷��B
		//0���w�肵�����͈ړ����Ȃ��B
		m_rigidBody->SetLinearFactor(DUMMY_RIGIDBODY_LINEARFACTOR, DUMMY_RIGIDBODY_LINEARFACTOR, DUMMY_RIGIDBODY_LINEARFACTOR);

		m_rigidBody->AddForce(cutForce, m_dummyModel->GetPosition());
		//���L�҂��Z�b�g
		m_dummyModel->SetOwner(this);
		return true;
	}

	void CutDummy::OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce)
	{
		//������̌��̃��f�����_�~�[�Ƃ��č쐬
		m_dummyModel->MakeDummy(cutForce);

		//�J�b�g���ꂽ�t���O���I��
		m_isCut = true;

		//�_�~�[�Ƃ��ĐV�����쐬���ꂽ�̂ŁA���g�̃N���X�͕s�v
		DeleteGO(this);
	}

	void CutDummy::Update()
	{
		//���Ԃ̃J�E���g�𑝉�
		m_timer++;

		if (m_timer < FRAME_DELETE - TIME_DITHER_START)
		{
			//���̂̍��W�Ɖ�]���󂯎��
			m_rigidBody->GetPositionAndRotation(m_position, m_rigidBodyRot);

			//���W�͂��̂܂܃��f���ɓK�p
			m_dummyModel->SetPosition(m_position);

			//rot�͏��������J�v�Z�������ւ̉�]
			//m_toModelRot�͂��̉�]���t�ɂ�����]�Ȃ̂ŁA
			//m_toModelRot����Z���鎖�ɂ���ď�Ƀ��f���͏�����ɂȂ�B
			m_rigidBodyRot.Multiply(m_toModelRot, m_rigidBodyRot);

			//�����Ɍ��̃��f���̉�]����Z���邱�ƂŃJ�v�Z���ƃ��f���̉�]����������
			m_rigidBodyRot.Multiply(m_modelRot, m_rigidBodyRot);

			//��]��K�p
			m_dummyModel->SetRotation(m_rigidBodyRot);
		}

		if (m_timer == FRAME_DELETE - TIME_DITHER_START)
		{
			//�f�B�U�����O���n�܂����獄�͍̂폜����
			delete m_rigidBody;
			m_rigidBody = nullptr;

			//�ؒf�s��
			m_dummyModel->SetDivideFlag(false);
		}

		//�������f�B�U�����O
		if (m_timer >= FRAME_DELETE - TIME_DITHER_START)
		{
			m_dummyModel->DitherProgress();
		}

		//�폜�t���[���ɂȂ�΍폜
		if (m_timer == FRAME_DELETE)
		{
			DeleteGO(this);
		}
	}
}