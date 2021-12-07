#pragma once
#include "../../ExEngine/physics/tkPhysicsGhostObject.h"

namespace Game
{
	class Player;

	class EnemyRock : public IGameObject
	{
	public:
		~EnemyRock();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* cutModel, const Vector3& cutForce) override;

		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		void SetDirection(const Vector3& direction)
		{
			m_moveDirection = direction;
			m_moveDirection.Normalize();
		}

	private:
		void PlayerHitTest();
	private:
		Player* m_targetPlayer = nullptr;
		Vector3 m_position;
		Quaternion m_qRot;
		Vector3 m_moveDirection;
		SkinModelRender* m_rockModel = nullptr;
		CPhysicsGhostObject m_sphereTrigger;
		bool m_isCut = false;
	};
}

