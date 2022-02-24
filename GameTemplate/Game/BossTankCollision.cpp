#include "stdafx.h"
#include "BossTankCollision.h"

namespace Game
{
	const Vector3 TANK_COLLISION_SIZE = { 400.0f,175.0f,800.0f };	//��Ԃ̓����蔻��̑傫��
	const float TANK_COLLISION_MASS = 100.0f;						//��Ԃ̓����蔻��̎���
	const float TANK_COLLISION_FRICTION = 10.0f;					//��Ԃ̓����蔻��̖��C��
	const Vector3 TANK_COLLISION_LINEARFACTOR = Vector3::Zero;		//��Ԃ̓����蔻��̈ړ��\�Ȏ�
}

namespace Game
{
	void BossTankCollision::Init(const Vector3& position, const Quaternion qRot)
	{
		//���̂��쐬
		m_boxCollider.Create(TANK_COLLISION_SIZE);
		RigidBodyInitData rbInitData;
		rbInitData.mass = TANK_COLLISION_MASS;
		rbInitData.collider = &m_boxCollider;
		rbInitData.pos = position;
		rbInitData.rot = qRot;

		//���̂̏�����
		m_rigidBody.Init(rbInitData);
		m_rigidBody.SetFriction(TANK_COLLISION_FRICTION);
		m_rigidBody.SetLinearFactor(TANK_COLLISION_LINEARFACTOR);
	}

	void BossTankCollision::Update(const Vector3& position, const Quaternion qRot)
	{
		//���̂̍��W�Ɖ�]���Z�b�g
		m_rigidBody.SetPositionAndRotation(position, qRot);
	}
}