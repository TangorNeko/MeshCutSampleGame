#pragma once
#include "../../ExEngine/physics/tkPhysicsGhostObject.h"

namespace Game
{
	class EnemyMissile : public IGameObject
	{
		~EnemyMissile();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* skinModelRender) override;

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
		SkinModelRender* m_missileRender = nullptr;		//�X�L�����f�������_�[
		Vector3 m_position = Vector3::Zero;				//���W
		Quaternion m_qRot = Quaternion::Identity;		//��]
		bool m_isCut = false;							//�J�b�g���ꂽ?
		CPhysicsGhostObject m_capsuleTrigger;
	};
}