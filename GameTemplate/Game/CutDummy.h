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

		void SetCapsuleRadius(float radius)
		{
			m_dummyRadius = radius;
		}

		void SetCapsuleHeight(float height)
		{
			m_dummyHeight = height;
		}
	private:
		SkinModelRender* m_dummyModel = nullptr;
		CCapsuleCollider m_capsuleCollider;
		float m_dummyRadius = 1.0f;
		float m_dummyHeight = 1.0f;
		RigidBody m_rigidBody;
	};
}

