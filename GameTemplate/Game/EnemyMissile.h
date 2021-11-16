#pragma once

namespace Game
{
	class EnemyMissile : public IGameObject
	{
		~EnemyMissile();

		bool Start() override;

		void Update() override;

		void OnDivide() override;

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

		/**
		 * @brief �_���[�W��^����(��ɍ폜�\��)
		 * @param damage �_���[�W
		*/
		void Damage(float damage);
	private:
		SkinModelRender* m_missileRender = nullptr;		//�X�L�����f�������_�[
		Vector3 m_position = Vector3::Zero;				//���W
		Quaternion m_qRot = Quaternion::Identity;		//��]
		float m_hp = 25.0f;								//�̗�(��ɍ폜�\��)
		FontRender* m_canCutTextRender = nullptr;		//�J�b�g�\�e�L�X�g(��ɍ폜�\��)
		bool m_isCut = false;							//�J�b�g���ꂽ?
	};
}