#pragma once

namespace Light
{
	struct DirLigData
	{
		Vector3 ligDir = { 1.0f,0.0f,0.0f };	//���C�g�̌���
		float pad0 = 0.0f;						//�p�f�B���O
		Vector3 ligColor = { 1.0f,1.0f,1.0f };	//���C�g�̐F
		float pad1 = 0.0f;						//�p�f�B���O(�P�̂��ƕK�v�Ȃ������ꂪ�Ȃ���DirLigData��z��ɂ�����HLSL��4�o�C�g������Ă������ƂɂȂ�?)
	};

	class DirectionLight : public IGameObject
	{
		~DirectionLight() override;
		bool Start() override;
		void Update() override;

	public:
		/**
		 * @brief �f�B���N�V�������C�g�̃f�[�^���擾
		 * @return �f�B���N�V�������C�g�̃f�[�^
		*/
		DirLigData* GetLigData() { return &m_dirLigData; }

		/**
		 * @brief �f�B���N�V�������C�g�̃f�[�^�̃T�C�Y���擾
		 * @return �f�B���N�V�������C�g�̃f�[�^�̃T�C�Y
		*/
		int GetLigDataSize() const { return sizeof(m_dirLigData); }

		/**
		 * @brief �f�B���N�V�������C�g�̌�����ݒ�
		 * @param dir �f�B���N�V�������C�g�̌���
		*/
		void SetDirection(const Vector3& dir) {
			m_dirLigData.ligDir = dir;
			m_dirLigData.ligDir.Normalize();
		}

		/**
		 * @brief �f�B���N�V�������C�g�̌������擾
		 * @return �f�B���N�V�������C�g�̌���
		*/
		const Vector3& GetDirecion() const { return m_dirLigData.ligDir; }

		/**
		 * @brief �f�B���N�V�������C�g�̐F��ݒ�
		 * @param color �f�B���N�V�������C�g�̐F
		*/
		void SetColor(const Vector3& color) { m_dirLigData.ligColor = color; }

		/**
		 * @brief �f�B���N�V�����̃��C�g�̐F���擾
		 * @return �f�B���N�V�������C�g�̐F
		*/
		const Vector3& GetColor() const { return m_dirLigData.ligColor; }

	private:
		DirLigData m_dirLigData;	//�f�B���N�V�������C�g�̃f�[�^
		int m_dirLigTag = 0;			//�f�B���N�V�������C�g�̔ԍ�(���Ԗڂɍ��ꂽ���C�g?)
	};
}

