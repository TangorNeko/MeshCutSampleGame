#pragma once

namespace Game
{
	class SpriteRender : public IGameObject
	{
	public:

		/**
		 * @brief �X�v���C�g�̕`��
		 * @param rc �����_�[�R���e�L�X�g
		*/
		void PostRender(RenderContext& rc) override;

		/**
		 * @brief �X�v���C�g�̏�����
		 * @param spritePath �X�v���C�g�̃p�X
		 * @param width �X�v���C�g�̉���
		 * @param height �X�v���C�g�̍���
		*/
		void Init(const char* spritePath, UINT width, UINT height);

		/**
		 * @brief �X�v���C�g�̍��W��ݒ�
		 * @param pos �X�v���C�g�̍��W
		*/
		void SetPosition(const Vector3& pos);

		/**
		 * @brief �X�v���C�g�̉�]��ݒ�
		 * @param qRot �X�v���C�g�̉�]
		*/
		void SetRotation(const Quaternion& qRot);

		/**
		 * @brief �X�v���C�g�̊g�嗦��ݒ�
		 * @param �g�嗦
		*/
		void SetScale(const Vector3& scale);

		/**
		 * @brief �X�v���C�g�̃s�{�b�g��ݒ�
		 * @param �s�{�b�g
		*/
		void SetPivot(const Vector2& pivot);

		/**
		 * @brief �X�v���C�g�̍��W���擾
		 * @return ���W
		*/
		const Vector3& GetPosition() const { return m_position; }

		/**
		 * @brief �X�v���C�g�̉�]���擾
		 * @return ��]
		*/
		const Quaternion& GetRotation() const { return m_qRot; }

		/**
		 * @brief �X�v���C�g�̊g�嗦���擾
		 * @return �g�嗦
		*/
		const Vector3& GetScale() const { return m_scale; }

		/**
		 * @brief �X�v���C�g�̃s�{�b�g���擾
		 * @return �s�{�b�g
		*/
		const Vector2& GetPivot() const { return m_pivot; }

		/**
		 * @brief �X�v���C�g�̏�Z�J���[��ݒ�
		 * @param mulColor ��Z�J���[
		*/
		void SetMulColor(const Vector4& mulColor)
		{
			m_sprite.SetMulColor(mulColor);
		}

	private:
		Sprite m_sprite;							//�X�v���C�g
		Vector3 m_position = Vector3::Zero;			//�X�v���C�g�̍��W
		Quaternion m_qRot = Quaternion::Identity;	//�X�v���C�g�̉�]
		Vector3 m_scale = Vector3::One;				//�X�v���C�g�̊g�嗦
		Vector2 m_pivot = { 0.5f,0.5f };			//�X�v���C�g�̃s�{�b�g
	};
}

