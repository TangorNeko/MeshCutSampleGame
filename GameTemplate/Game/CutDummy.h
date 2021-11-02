#pragma once

namespace Game
{
	class CutDummy : public IGameObject
	{
	public:
		~CutDummy();
	
		bool Start();

		void Update();

		void SetSkinModel(SkinModelRender* modelRender)
		{
			m_dummyModel = modelRender;
		}

		void SetSphereRadius(float radius)
		{
			m_dummyRadius = radius;
		}
	private:
		SkinModelRender* m_dummyModel = nullptr;
		SphereCollider m_sphereCollider;
		float m_dummyRadius = 1.0f;
		RigidBody m_rigidBody;
	};
}

