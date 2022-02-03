#pragma once

namespace Game
{
	class BossTankCollision
	{
	public:
		/**
		 * @brief �����蔻��̏�����
		 * @param position �{�X�̍��W
		 * @param qRot �{�X�̉�]
		*/
		void Init(const Vector3& position,const Quaternion qRot);

		/**
		 * @brief �����蔻��̍X�V
		 * @param position �{�X�̍��W
		 * @param qRot �{�X�̉�]
		*/
		void Update(const Vector3& position, const Quaternion qRot);
	private:
		BoxCollider m_boxCollider;	//�{�b�N�X�̓����蔻��
		RigidBody m_rigidBody;		//����
	};
}

