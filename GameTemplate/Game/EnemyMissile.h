#pragma once
#include "../../ExEngine/physics/tkPhysicsGhostObject.h"
#include "EnemyMissileMove.h"
#include "EnemyMissileRotation.h"

namespace Game
{
	class EnemyMissile : public IGameObject
	{
		~EnemyMissile();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce) override;

		/**
		 * @brief �v���C���[�Ƃ̓����蔻��
		*/
		void PlayerHitTest();

	public:
		/**
		 * @brief ���W���Z�b�g
		 * @param position ���W
		*/
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		/**
		 * @brief ���W���擾
		 * @return ���W
		*/
		const Vector3& GetPosition()
		{
			return m_position;
		}
		
		/**
		 * @brief ��]���Z�b�g
		 * @param rot ��]
		*/
		void SetRotation(const Quaternion rot)
		{
			m_qRot = rot;
		}
	private:
		Player* m_trackingPlayer = nullptr;				//�ǔ�����v���C���[
		SkinModelRender* m_missileRender = nullptr;		//�X�L�����f�������_�[
		Vector3 m_position = Vector3::Zero;				//���W
		Quaternion m_qRot = Quaternion::Identity;		//��]
		bool m_isCut = false;							//�J�b�g���ꂽ?
		CPhysicsGhostObject m_capsuleTrigger;			//�����蔻��̃g���K�[
		EnemyMissileMove m_missileMove;					//�~�T�C���̈ړ������N���X
		EnemyMissileRotation m_missileRotation;			//�~�T�C���̉�]�����N���X
		Effect* m_missileEffect = nullptr;
	};
}