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
		SkinModelRender* m_baseRender = nullptr;		//���f��
		Vector3 m_position = Vector3::Zero;				//���W
		Quaternion m_baseRot = Quaternion::Identity;	//��]
		Vector3 m_moveDirection = Vector3::Zero;		//�ړ�����
		int m_actionFrame = 0;							//�s���̌o�߃t���[��
		bool m_isDead = false;							//���S����?
	};
}

