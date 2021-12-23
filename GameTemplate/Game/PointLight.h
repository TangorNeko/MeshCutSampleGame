#pragma once

namespace Light
{
	struct PointLigData
	{
		Vector3 ligPos = { 0.0f,0.0f,0.0f };	//���C�g�̍��W
		float pad = 0.0f;						//�p�f�B���O
		Vector3 ligColor = { 1.0f,1.0f,1.0f };	//���C�g�̐F
		float ligRange = 0.0f;					//���C�g�̉e���͈�
	};

	class PointLight : public IGameObject
	{
		~PointLight()override;
		bool Start()override;
		void Update()override;

	public:
		/**
		 * @brief �|�C���g���C�g�̃f�[�^���擾
		 * @return �|�C���g���C�g�̃f�[�^
		*/
		PointLigData* GetLigData() { return &m_pointLigData; }

		/**
		 * @brief �|�C���g���C�g�̃f�[�^�̃T�C�Y���擾
		 * @return �|�C���g���C�g�̃f�[�^�̃T�C�Y
		*/
		int GetLigDataSize() const { return sizeof(PointLigData); }

		/**
		 * @brief �|�C���g���C�g�̍��W��ݒ�
		 * @param pos �|�C���g���C�g�̍��W
		*/
		void SetPosition(const Vector3& pos) { m_pointLigData.ligPos = pos; }

		/**
		 * @brief �|�C���g���C�g�̍��W���擾
		 * @return �|�C���g���C�g�̍��W
		*/
		const Vector3& GetPosition() const { return m_pointLigData.ligPos; }

		/**
		 * @brief �|�C���g���C�g�̐F��ݒ�
		 * @param color �|�C���g���C�g�̐F
		*/
		void SetColor(const Vector3& color) { m_pointLigData.ligColor = color; }

		/**
		 * @brief �|�C���g���C�g�̐F���擾
		 * @return �|�C���g���C�g�̐F
		*/
		const Vector3& GetColor() const { return m_pointLigData.ligColor; }

		/**
		 * @brief �|�C���g���C�g�̉e���͈͂�ݒ�
		 * @param range �|�C���g���C�g�̉e���͈�
		*/
		void SetRange(float range) { m_pointLigData.ligRange = range; }

		/**
		 * @brief �|�C���g���C�g�̉e���͈͂��擾
		 * @return �|�C���g���C�g�̉e���͈�
		*/
		float GetRange() const { return m_pointLigData.ligRange; }

	private:
		PointLigData m_pointLigData;	//�|�C���g���C�g�̃f�[�^
		int m_pointLigTag = 0;				//�|�C���g���C�g�̃^�O(���Ԗڂɍ��ꂽ���C�g?)
	};
}
