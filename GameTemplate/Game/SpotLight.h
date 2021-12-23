#pragma once

namespace Light
{
	struct SpotLigData //�X�|�b�g���C�g�̃f�[�^
	{
		Vector3 ligPos = { 0.0f,0.0f,0.0f };	//���C�g�̍��W
		float pad = 0.0f;						//�p�f�B���O
		Vector3 ligColor = { 1.0f,1.0f,1.0f };	//���C�g�̐F
		float ligRange = 0.0f;					//���C�g�̉e���͈�
		Vector3 ligDir = { 1.0f,0.0f,0.0f };	//���C�g�̌���
		float ligAngle = 0.0f;					//���C�g�̉e���p�x(���W�A���P��)
	};

	class SpotLight : public IGameObject
	{
		~SpotLight()override;
		bool Start()override;
		void Update()override;

	public:
		/**
		 * @brief �X�|�b�g���C�g�̃f�[�^���擾
		 * @return �X�|�b�g���C�g�̃f�[�^
		*/
		SpotLigData* GetLigData() { return &m_spotLigData; }

		/**
		 * @brief �X�|�b�g���C�g�̃f�[�^�̃T�C�Y���擾
		 * @return �X�|�b�g���C�g�̃f�[�^�̃T�C�Y
		*/
		int GetLigDataSize() const { return sizeof(SpotLigData); }

		/**
		 * @brief �X�|�b�g���C�g�̍��W��ݒ�
		 * @param pos �X�|�b�g���C�g�̍��W
		*/
		void SetPosition(const Vector3& pos) { m_spotLigData.ligPos = pos; }

		/**
		 * @brief �X�|�b�g���C�g�̍��W���擾
		 * @return �X�|�b�g���C�g�̍��W
		*/
		const Vector3& GetPosition() const { return m_spotLigData.ligPos; }

		/**
		 * @brief �X�|�b�g���C�g�̐F��ݒ�
		 * @param color �X�|�b�g���C�g�̐F
		*/
		void SetColor(const Vector3& color) { m_spotLigData.ligColor = color; }

		/**
		 * @brief �X�|�b�g���C�g�̐F���擾
		 * @return �X�|�b�g���C�g�̐F
		*/
		const Vector3& GetColor() const { return m_spotLigData.ligColor; }

		/**
		 * @brief �X�|�b�g���C�g�̉e���͈͂�ݒ�
		 * @param range �X�|�b�g���C�g�̉e���͈�
		*/
		void SetRange(float range) { m_spotLigData.ligRange = range; }

		/**
		 * @brief �X�|�b�g���C�g�̉e���͈͂��擾
		 * @return �X�|�b�g���C�g�̉e���͈�
		*/
		float GetRange() const { return m_spotLigData.ligRange; }

		/**
		 * @brief �X�|�b�g���C�g�̌�����ݒ�
		 * @param dir �X�|�b�g���C�g�̌���
		*/
		void SetDirection(const Vector3& dir) { m_spotLigData.ligDir = dir; }

		/**
		 * @brief �X�|�b�g���C�g�̌������擾
		 * @return �X�|�b�g���C�g�̌���
		*/
		const Vector3& GetDirection() const { return m_spotLigData.ligDir; }

		/**
		 * @brief �X�|�b�g���C�g�̉e���p�x��ݒ�(���W�A���P��)
		 * @param angle_rad �X�|�b�g���C�g�̉e���p�x
		*/
		void SetAngle(float angle_rad) { m_spotLigData.ligAngle = angle_rad; }

		/**
		 * @brief �X�|�b�g���C�g�̉e���p�x���擾(���W�A���P��)
		 * @return �X�|�b�g���C�g�̉e���p�x
		*/
		float GetAngle() const { return m_spotLigData.ligAngle; }

		/**
		 * @brief �X�|�b�g���C�g�̉e���p�x��ݒ�(�f�O���[�P��)
		 * @param angle_deg �X�|�b�g���C�g�̉e���p�x
		*/
		void SetAngleDeg(float angle_deg) { m_spotLigData.ligAngle = Math::DegToRad(angle_deg); }

		/**
		 * @brief �X�|�b�g���C�g�̉e���p�x���擾(�f�O���[�P��)
		 * @return �X�|�b�g���C�g�̉e���p�x
		*/
		float GetAngleDeg() const { return Math::RadToDeg(m_spotLigData.ligAngle); }

	private:
		SpotLigData m_spotLigData;	//�X�|�b�g���C�g�̃f�[�^
		int m_spotLigTag = 0;			//�X�|�b�g���C�g�̃^�O(���Ԗڂɍ��ꂽ���C�g?)
	};
}

