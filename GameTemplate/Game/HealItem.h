#pragma once
namespace Game
{
	class Player;

	class HealItem : public IGameObject
	{
	public:

		~HealItem();

		bool Start() override;

		void Update() override;
		
		/**
		 * @brief ���W�̐ݒ�
		 * @param position ���W
		*/
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		/**
		 * @brief �E���鏈��
		*/
		void PickedUp();
	private:
		Vector3 m_position = Vector3::Zero;				//���W
		Quaternion m_qRot = Quaternion::Identity;		//��]
		float m_rotateDegY = 0;							//Y������̉�]�p�x
		SkinModelRender* m_skinModelRender = nullptr;	//���f�������_�[
		Player* m_player = nullptr;						//�E����v���C���[
		BoxCollider m_boxCollider;						//���̓����蔻��
		RigidBody m_rigidBody;							//����
	};
}

