#pragma once

namespace Game
{
	class StepObject : public IGameObject
	{
	public:
		~StepObject();

		bool Start() override;

		void Update() override;

		/**
		 * @brief ���W��ݒ肷��
		 * @param position ���W
		*/
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		/**
		 * @brief ���W���擾����
		 * @return ���W
		*/
		const Vector3& GetPosition()
		{
			return m_position;
		}

		/**
		 * @brief �ړ��������w�肷��
		 * @param moveDirection 
		*/
		void SetMoveDirection(const Vector3& moveDirection)
		{
			m_moveDirection = moveDirection;
			m_moveDirection.Normalize();
		}
	private:
		Quaternion m_qRot;								//���f���̉�]
		Vector3 m_moveDirection = Vector3::AxisZ;		//�ړ�����
		Vector3 m_position =  Vector3::Zero;			//���W
		SkinModelRender* m_skinModelRender = nullptr;	//�X�L�����f�������_�[
		int m_lifeTime = 0;								//��������
		Effect* m_missileEffect = nullptr;				//�~�T�C���̃G�t�F�N�g
	};
}

