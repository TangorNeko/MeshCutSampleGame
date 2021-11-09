#pragma once

namespace Game
{
	class BackGround : public IGameObject
	{
		~BackGround();

		bool Start() override;

		void Update() override;

	private:
		SkinModelRender* m_stageModel = nullptr;	//�X�e�[�W�̃��f��
		PhysicsStaticObject m_physicsStaticObject;	//�X�e�[�W�̃��f���̓����蔻��
	};
}

