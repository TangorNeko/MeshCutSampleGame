#pragma once

namespace Game
{
	class Player;
	class Door : public IGameObject
	{
	public:
		~Door();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* cutModel, const Vector3& cutForce) override;

	private:
		SkinModelRender* m_doorModel = nullptr;					//�h�A�̃��f��
		PhysicsStaticObject* m_physicsStaticObject = nullptr;	//�h�A�̃��f���̓����蔻��
		Player* m_player = nullptr;								//�v���C���[�N���X�̃|�C���^
		bool m_isDisplayAttackTutorial = false;					//�U���̃`���[�g���A����\������?
	};
}