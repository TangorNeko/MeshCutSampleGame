#include "stdafx.h"
#include "CutDummy.h"

namespace Game
{
	CutDummy::~CutDummy()
	{
		//�J�b�g����Ă��Ȃ������烂�f�����폜
		if (m_isCut == false)
		{
			DeleteGO(m_dummyModel);
		}
	}

	bool CutDummy::Start()
	{
		//�J�v�Z����̃R���C�_�[���쐬
		m_capsuleCollider.Init(m_dummyRadius/1.5f,m_dummyHeight/1.5f);
		
		//����̃R���C�_�[���쐬
		RigidBodyInitData rbInitData;

		//�d�ʂ��Z�b�g(�T���v���̂܂�)
		rbInitData.mass = 0.1f;

		//�R���C�_�[���Z�b�g
		rbInitData.collider = &m_capsuleCollider;

		//���f���̉�]��ۑ����Ă���
		m_modelRot = m_dummyModel->GetRotation();

		//�������W�Ɖ�]���Z�b�g
		rbInitData.pos = m_dummyModel->GetPosition();
		rbInitData.rot = m_capsuleRot;

		//��]�̂��₷����ݒ肷��B(0�`1�A�T���v���̂܂�)
		rbInitData.localInteria.Set(
			0.1f,
			0.1f,
			0.1f
		);

		m_rigidBody.Init(rbInitData);
		//���C�͂�ݒ肷��B(0�`10�A�T���v���̂܂�)
		m_rigidBody.SetFriction(10.0f);
		//���`�ړ�����v�f��ݒ肷��B
		//0���w�肵�����͈ړ����Ȃ��B
		m_rigidBody.SetLinearFactor(1.0f, 1.0f, 1.0f);

		//���L�҂��Z�b�g
		m_dummyModel->SetOwner(this);
		return true;
	}

	void CutDummy::OnDivide(const SkinModelRender* skinModelRender)
	{
		//������̌��̃��f�����_�~�[�Ƃ��č쐬
		m_dummyModel->MakeDummy();

		//�J�b�g���ꂽ�t���O���I��
		m_isCut = true;

		//�_�~�[�Ƃ��ĐV�����쐬���ꂽ�̂ŁA���g�̃N���X�͕s�v
		DeleteGO(this);
	}

	void CutDummy::Update()
	{
		Vector3 pos;
		Quaternion rot;
		
		//���̂̍��W�Ɖ�]���󂯎��
		m_rigidBody.GetPositionAndRotation(pos, rot);

		//���W�͂��̂܂܃��f���ɓK�p
		m_dummyModel->SetPosition(pos);

		//rot�͏��������J�v�Z�������ւ̉�]
		//m_toModelRot�͂��̉�]���t�ɂ�����]�Ȃ̂ŁA
		//m_toModelRot����Z���鎖�ɂ���ď�Ƀ��f���͏�����ɂȂ�B
		rot.Multiply(m_toModelRot,rot);

		//�����Ɍ��̃��f���̉�]����Z���邱�ƂŃJ�v�Z���ƃ��f���̉�]����������
		rot.Multiply(m_modelRot, rot);
		
		//��]��K�p
		m_dummyModel->SetRotation(rot);

		//���Ԃ̌o�߂Ń_�~�[���폜
		m_timer++;
		if (m_timer == 360)
		{
			DeleteGO(this);
		}
	}
}