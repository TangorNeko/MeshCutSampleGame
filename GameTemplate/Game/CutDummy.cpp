#include "stdafx.h"
#include "CutDummy.h"

namespace Game
{
	CutDummy::~CutDummy()
	{

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
		return true;
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
	}
}