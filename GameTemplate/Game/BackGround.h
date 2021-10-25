#pragma once

namespace Game
{
	class BackGround : public IGameObject
	{
		~BackGround();

		bool Start() override;

		void Update() override;

	private:
		SkinModelRender* m_stageModel = nullptr;
		PhysicsStaticObject m_physicsStaticObject;
		Vector3 m_position = Vector3::Zero;
	};
}

