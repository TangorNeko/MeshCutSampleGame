#pragma once

namespace Game
{
	class MiniEnemy : public IGameObject
	{
	public:
		~MiniEnemy();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce) override;

		/**
		 * @brief ���W��ݒ�
		 * @param position 
		*/
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
	private:
		SkinModelRender* m_baseRender = nullptr;		//�ԑ̂̃��f��
		SkinModelRender* m_turretRender = nullptr;		//�C���̃��f��
		SkinModelRender* m_cannonRender = nullptr;		//�C�g�̃��f��
		Vector3 m_position = Vector3::Zero;				//���W
		Quaternion m_baseRot = Quaternion::Identity;	//�ԑ̂̉�]
		Quaternion m_turretRot = Quaternion::Identity;	//�C���̉�]
		Vector3 m_moveDirection = Vector3::Zero;		//�ړ�����
		int m_actionFrame = 0;							//�s���̌o�߃t���[��
		float m_turretDeg = 0.0f;						//�C���̊p�x
		bool m_isDead = false;							//���S����?
		bool m_isBaseBreak = false;						//�ԑ̂��j�󂳂ꂽ?
		bool m_isTurretBreak = false;					//�C�����j�󂳂ꂽ?
		bool m_isCannonBreak = false;					//�C�g���j�󂳂ꂽ?
	};
}

