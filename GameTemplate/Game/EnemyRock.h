#pragma once
#include "../../ExEngine/physics/tkPhysicsGhostObject.h"

namespace Game
{
	class Player;

	class EnemyRock : public IGameObject
	{
	public:
		~EnemyRock();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* cutModel, const Vector3& cutForce) override;

		/**
		 * @brief ���W��ݒ�
		 * @param position 
		*/
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		/**
		 * @brief �ړ�������ݒ�
		 * @param direction 
		*/
		void SetDirection(const Vector3& direction)
		{
			m_moveDirection = direction;
			m_moveDirection.Normalize();
		}

	private:
		/**
		 * @brief �v���C���[�Ƃ̓����蔻��
		*/
		void PlayerHitTest();
	private:
		Player* m_targetPlayer = nullptr;			//�_���[�W��^����v���C���[
		Vector3 m_position = Vector3::Zero;			//���W
		Quaternion m_qRot = Quaternion::Identity;	//��]
		Vector3 m_moveDirection = Vector3::Front;	//�ړ�����
		SkinModelRender* m_rockModel = nullptr;		//��̃��f��
		CPhysicsGhostObject m_sphereTrigger;		//����̓����蔻��
		bool m_isCut = false;						//�ؒf���ꂽ?
	};
}

