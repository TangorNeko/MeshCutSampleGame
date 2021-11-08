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
		//m_capsuleCollider.Init(m_dummyRadius,m_dummyHeight * 2);
		
		//����̃R���C�_�[���쐬
		m_sphereCollider.Create(m_dummyRadius);
		RigidBodyInitData rbInitData;

		//�d�ʂ��Z�b�g(�T���v���̂܂�)
		rbInitData.mass = 0.1f;

		//�R���C�_�[���Z�b�g
		//rbInitData.collider = &m_capsuleCollider;
		rbInitData.collider = &m_sphereCollider;

		Quaternion colliderRot;
		colliderRot.Multiply(m_dummyModel->GetRotation(), colliderRot);
		colliderRot.Multiply(m_capsuleRot, colliderRot);
		//�������W�Ɖ�]���Z�b�g
		rbInitData.pos = m_dummyModel->GetPosition();
		rbInitData.rot = colliderRot;

		//��]�̂��₷����ݒ肷��B(0�`1�A�T���v���̂܂�)
		rbInitData.localInteria.Set(
			0.5f,
			0.5f,
			0.5f
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
		m_rigidBody.GetPositionAndRotation(pos, rot);

		m_dummyModel->SetPosition(pos);
		rot.Multiply(m_toModelRot);
		m_dummyModel->SetRotation(rot);
	}
}