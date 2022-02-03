#pragma once

namespace Game
{
	class CutDummy : public IGameObject
	{
	public:
		~CutDummy();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce) override;

		/**
		 * @brief �X�L�����f���𒼐ړo�^����
		 * @param modelRender �X�L�����f�������_�[
		*/
		void SetSkinModel(SkinModelRender* modelRender)
		{
			m_dummyModel = modelRender;
		}

		/**
		 * @brief �_�~�[�Ɏg�p����J�v�Z���̔��a��o�^
		 * @param radius 
		*/
		void SetCapsuleRadius(float radius)
		{
			m_dummyRadius = radius;
		}

		//�_�~�[�Ɏg�p����J�v�Z���̍�����o�^
		void SetCapsuleHeight(float height)
		{
			m_dummyHeight = height;
		}

		/**
		 * @brief ������̎�����J�v�Z���̎��ւ̉�]�N�H�[�^�j�I���ƁA���̉�]��߂��N�H�[�^�j�I�����Z�b�g����
		 * @param capsuleRot �J�v�Z���̎������ɉ�]������N�H�[�^�j�I��
		 * @param toModelRot �J�v�Z���̎��������������̎��ɖ߂��N�H�[�^�j�I��
		*/
		void SetRotations(const Quaternion& capsuleRot, const Quaternion& toModelRot)
		{
			m_capsuleRot = capsuleRot;
			m_toModelRot = toModelRot;
		}

		/**
		 * @brief �ؒf���̗͂��Z�b�g����
		 * @param force �ؒf���̗�
		*/
		void SetCutForce(const Vector3& force)
		{
			cutForce = force;
		}
	private:
		SkinModelRender* m_dummyModel = nullptr;			//�_�~�[�p�̃��f��
		CapsuleCollider m_capsuleCollider;					//�J�v�Z����̃R���C�_�[
		float m_dummyRadius = 1.0f;							//�_�~�[�̃J�v�Z���̔��a
		float m_dummyHeight = 1.0f;							//�_�~�[�̃J�v�Z���̍���
		RigidBody* m_rigidBody = nullptr;					//�_�~�[�̍���
		Vector3 m_position = Vector3::Zero;					//���W
		Quaternion m_rigidBodyRot = Quaternion::Identity;	//���̂̉�];
		Quaternion m_capsuleRot = Quaternion::Identity;		//���������J�v�Z���̎������։�]������N�H�[�^�j�I��
		Quaternion m_toModelRot = Quaternion::Identity;		//�J�v�Z���̎��������������։�]������(�J�v�Z�����ւ̉�]��߂�)�N�H�[�^�j�I��
		Quaternion m_modelRot = Quaternion::Identity;		//���X�̃��f���̃N�H�[�^�j�I��
		bool m_isCut = false;								//�ؒf���ꂽ?
		int m_timer = 0;									//���Ń^�C�}�[
		Vector3 cutForce = Vector3::Zero;					//�ؒf���̗�
	};
}

