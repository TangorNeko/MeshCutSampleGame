#pragma once

namespace Game
{
	class BossTankCollision
	{
	public:
		void Init(const Vector3& position,const Quaternion qRot);

		void Update(const Vector3& position, const Quaternion qRot);
	private:
		BoxCollider m_boxCollider;
		RigidBody m_rigidBody;
	};
}

