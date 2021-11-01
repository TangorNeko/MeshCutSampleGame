#pragma once

namespace Game
{
	class CutDummy : public IGameObject
	{
	public:
		~CutDummy();
	
		bool Start();

		void Update();
	private:
		SkinModelRender* m_dummyModel = nullptr;
		Vector3 m_position;
		Quaternion m_qRot;
		SphereCollider m_sphereCollider;
		RigidBody m_rigidBody;
	};
}

