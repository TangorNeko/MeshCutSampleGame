#pragma once

namespace Game
{
	class BackGround : public IGameObject
	{
		~BackGround();

		bool Start() override;

		void Update() override;

	private:
		SkinModelRender* m_stageModel = nullptr;	//ステージのモデル
		PhysicsStaticObject m_physicsStaticObject;	//ステージのモデルの当たり判定
	};
}

