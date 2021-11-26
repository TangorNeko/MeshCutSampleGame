#include "stdafx.h"
#include "BossTankCollision.h"

namespace Game
{
	void BossTankCollision::Init(const Vector3& position, const Quaternion qRot)
	{
		m_boxCollider.Create({ 400.0f,175.0f,800.0f });
		RigidBodyInitData rbInitData;
		rbInitData.mass = 100.0f;
		rbInitData.collider = &m_boxCollider;
		rbInitData.pos = position;
		rbInitData.rot = qRot;

		rbInitData.localInteria.Set(
			0.0f,
			0.0f,
			0.0f
		);

		//çÑëÃÇÃèâä˙âª
		m_rigidBody.Init(rbInitData);
		m_rigidBody.SetFriction(10.0f);
		m_rigidBody.SetLinearFactor(0.0f, 0.0f, 0.0f);
	}

	void BossTankCollision::Update(const Vector3& position, const Quaternion qRot)
	{
		m_rigidBody.SetPositionAndRotation(position, qRot);
	}
}