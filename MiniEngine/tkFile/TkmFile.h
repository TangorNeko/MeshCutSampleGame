/// <summary>
/// tkm�t�@�C���B
/// </summary>
/// <remarks>
/// tkm�t�@�C����3D���f���t�H�[�}�b�g�ł��B
/// ���̃N���X�𗘗p���邱�Ƃ�tkm�t�@�C�����������Ƃ��ł��܂��B�B
/// </remarks>

#pragma once

#include "../../GameTemplate/Game/MakeHashFromString.h"
	
/// <summary>
/// tkm�t�@�C���N���X�B
/// </summary>
class  TkmFile {
public:
	/// <summary>
	/// �}�e���A��
	/// </summary>
	struct SMaterial {
		std::string albedoMapFileName;			//�A���x�h�}�b�v�̃t�@�C�����B
		std::string normalMapFileName;			//�@���}�b�v�̃t�@�C�����B
		std::string specularMapFileName;		//�X�y�L�����}�b�v�̃t�@�C�����B
		std::string reflectionMapFileName;		//���t���N�V�����}�b�v�̃t�@�C�����B
		std::string refractionMapFileName;		//���܃}�b�v�̃t�@�C�����B
		std::unique_ptr<char[]>	albedoMap = nullptr;		//���[�h���ꂽ�A���x�h�}�b�v�B(dds�t�@�C��)
		unsigned int albedoMapSize = 0;				//�A���x�h�}�b�v�̃T�C�Y�B(dds�t�@�C��)
		std::unique_ptr<char[]>	normalMap = nullptr;		//���[�h���ꂽ�@���}�b�v�B(dds�t�@�C��)
		unsigned int normalMapSize = 0;				//�@���}�b�v�̃T�C�Y�B
		std::unique_ptr<char[]>	specularMap = nullptr;	//���[�h���ꂽ�X�y�L�����}�b�v�B(dds�t�@�C��)
		unsigned int specularMapSize = 0;			//�X�y�L�����}�b�v�̃T�C�Y�B(dds�t�@�C��)
		std::unique_ptr<char[]>	reflectionMap = nullptr;	//���[�h���ꂽ���t���N�V�����}�b�v�B(dds�t�@�C��)
		unsigned int reflectionMapSize = 0;			//���t���N�V�����}�b�v�̃T�C�Y�B(dds�t�@�C��)
		std::unique_ptr<char[]>	refractionMap = nullptr;	//���[�h���ꂽ���܃}�b�v�B(dds�t�@�C��)
		unsigned int refractionMapSize = 0;			//���܃}�b�v�̃T�C�Y�B(dds�t�@�C��)

		//NOTE:��������̓��b�V���̕������}�e���A�����R�s�[�ł���悤�ɏ����Ă݂����ł��B
		//���삪�S���ۏ؂���Ă��Ȃ��̂Œ��ӂ��Ă��������B

		//�f�t�H���g�R���X�g���N�^
		SMaterial() {}

		//�R�s�[�R���X�g���N�^
		SMaterial(const SMaterial& rhs)
		{
			this->CopyFrom(rhs);
		}
		void CopyFrom(const SMaterial& rhs)
		{
			albedoMapFileName = rhs.albedoMapFileName;
			normalMapFileName = rhs.normalMapFileName;
			specularMapFileName = rhs.specularMapFileName;
			reflectionMapFileName = rhs.reflectionMapFileName;
			refractionMapFileName = rhs.refractionMapFileName;

			albedoMapSize = rhs.albedoMapSize;
			albedoMap = std::make_unique<char[]>(albedoMapSize);
			memcpy(albedoMap.get(), rhs.albedoMap.get(), albedoMapSize);
			normalMapSize = rhs.normalMapSize;
			normalMap = std::make_unique<char[]>(normalMapSize);
			memcpy(normalMap.get(), rhs.normalMap.get(), normalMapSize);
			specularMapSize = specularMapSize;
			specularMap = std::make_unique<char[]>(specularMapSize);
			memcpy(specularMap.get(), rhs.specularMap.get(), specularMapSize);
			reflectionMapSize = reflectionMapSize;
			reflectionMap = std::make_unique<char[]>(reflectionMapSize);
			memcpy(reflectionMap.get(), rhs.reflectionMap.get(), reflectionMapSize);
			refractionMapSize = refractionMapSize;
			refractionMap = std::make_unique<char[]>(refractionMapSize);
			memcpy(refractionMap.get(), rhs.refractionMap.get(), refractionMapSize);

		}
		//�R�s�[������Z�q
		void operator=(const SMaterial& rhs)
		{
			this->CopyFrom(rhs);
		}
	};
	/// <summary>
	/// ���_�B
	/// </summary>
	/// <remarks>
	/// ������ҏW������A���C�g���̃V�F�[�_�[�Œ�`����Ă��钸�_�\���̂��ύX����K�v������B
	/// </remarks>
	struct SVertex {
		Vector3 pos;			//���W�B
		Vector3 normal;		//�@���B
		Vector3 tangent;		//�ڃx�N�g���B
		Vector3 binormal;		//�]�x�N�g���B
		Vector2 uv;			//UV���W�B
		int indices[4];			//�X�L���C���f�b�N�X�B
		Vector4 skinWeights;	//�X�L���E�F�C�g�B

		/**
		 * @brief map�o�^�p��SVertex�̔�r���Z�q �ʏ�̔�r�Ƃ��Ă͎g���܂���B
		 * @param val ��r�Ώ�
		 * @return ��r�Ώۂ̃n�b�V���l��菬����?
		*/
		bool operator<(const SVertex& rhs) const
		{
			MakeHashFromString hashMaker;
			char Buffer[256];
			char rhsBuffer[256];
			sprintf_s(Buffer, "x=%.2fy=%.2fz=%.2fuvx=%.2fuvy=%.2f", pos.x, pos.y, pos.z, uv.x, uv.y);
			sprintf_s(rhsBuffer, "x=%.2fy=%.2fz=%.2fuvx=%.2fuvy=%.2f", rhs.pos.x, rhs.pos.y, rhs.pos.z, rhs.uv.x, rhs.uv.y);
			int Hash = hashMaker.MakeHash(Buffer);
			int rhsHash = hashMaker.MakeHash(rhsBuffer);
			return Hash < rhsHash;
		}
	};

	static SVertex lerpVertex(float lerpRate,const SVertex& v0, const SVertex& v1)
	{
		SVertex vertex;
		vertex.pos.Lerp(lerpRate, v0.pos, v1.pos);
		vertex.normal.Lerp(lerpRate, v0.normal, v1.normal);
		vertex.tangent.Lerp(lerpRate, v0.tangent, v1.tangent);
		vertex.binormal.Lerp(lerpRate, v0.binormal, v1.binormal);
		vertex.uv.Lerp(lerpRate, v0.uv, v1.uv);

		//WARNING:�X�L���C���f�b�N�X��艺�̕⊮�̎d����������Ȃ�(���������⊮���Ă�����?)
		//�̂ŃX�L���C���f�b�N�X��v0�̃C���f�b�N�X���A�X�L���E�F�C�g�͍쐬����Lerp�֐����g���Ă݂�
		vertex.indices[0] = v0.indices[0];
		vertex.indices[1] = v0.indices[1];
		vertex.indices[2] = v0.indices[2];
		vertex.indices[3] = v0.indices[3];
		vertex.skinWeights.Lerp(lerpRate, v0.skinWeights, v1.skinWeights);

		return vertex;
	}
	/// <summary>
	/// 32�r�b�g�̃C���f�b�N�X�o�b�t�@�B
	/// </summary>
	struct SIndexBuffer32 {
		std::vector< uint32_t > indices;	//�C���f�b�N�X�B
	};
	/// <summary>
	/// 16�r�b�g�̃C���f�b�N�X�o�b�t�@�B
	/// </summary>
	struct SIndexbuffer16 {
		std::vector< uint16_t > indices;	//�C���f�b�N�X�B
	};
	/// <summary>
	/// ���b�V���p�[�c�B
	/// </summary>
	struct SMesh {
		bool isFlatShading;							//�t���b�g�V�F�[�f�B���O�H
		std::vector< SMaterial > materials;				//�}�e���A���̔z��B
		std::vector< SVertex >	vertexBuffer;			//���_�o�b�t�@�B
		std::vector<SIndexBuffer32> indexBuffer32Array;	//�C���f�b�N�X�o�b�t�@�̔z��B�}�e���A���̐��������C���f�b�N�X�o�b�t�@�͂����B
		std::vector< SIndexbuffer16> indexBuffer16Array;
	};
		
	/// <summary>
	/// 3D���f�������[�h�B
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X�B</param>
	void Load(const char* filePath);

	/**
	 * @brief ���b�V����ǉ�
	 * @param mesh �ǉ����郁�b�V��
	*/
	void AddMesh(SMesh mesh)
	{
		m_meshParts.push_back(mesh);
	}
		
	/// <summary>
	/// ���b�V���p�[�c�ɑ΂��ăN�G�����s���B
	/// </summary>
	/// <param name="func">�N�G���֐�</param>
	void QueryMeshParts(std::function<void(const SMesh& mesh)> func) const
	{
		for (auto& mesh : m_meshParts) {
			func(mesh);
		}
	}
	/// <summary>
	/// ���b�V���p�[�c���擾�B
	/// </summary>
	/// <returns></returns>
	const std::vector< SMesh>& GetMeshParts() const
	{
		return m_meshParts;
	}
	/// <summary>
	/// ���b�V���̐����擾�B
	/// </summary>
	/// <returns></returns>
	int GetNumMesh() const
	{
		return (int)(m_meshParts.size());
	}

	/**
	 * @brief tkMFile���̃��f����ؒf�ؒf
	 * @param cutNormal �ؒf�ʂ̈�_����̖@���̌���
	 * @param cutPoint �ؒf�ʂ̈�_�̍��W
	 * @return �����̃��b�V���̃��b�V���p�[�c
	*/
	std::vector< SMesh> Divide(const Vector3& cutNormal, const Vector3& cutPoint);

	void SetMeshParts(const std::vector<SMesh> meshParts)
	{
		m_meshParts = meshParts;
	}
private:
	/// <summary>
	/// �e�N�X�`���������[�h�B
	/// </summary>
	/// <param name="fp"></param>
	/// <returns></returns>
	std::string LoadTextureFileName(FILE* fp);
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�����[�h�B
	/// </summary>
	template<class T>
	void LoadIndexBuffer(std::vector<T>& indexBuffer, int numIndex, FILE* fp);
	/// <summary>
	/// �}�e���A�����\�z�B
	/// </summary>
	/// <param name="tkmMat"></param>
	void BuildMaterial(SMaterial& tkmMat, FILE* fp, const char* filePath);
	/// <summary>
	/// �ڃx�N�g���Ə]�x�N�g�����v�Z����B
	/// </summary>
	/// <remarks>
	/// 3dsMaxScript�ł��ׂ��Ȃ񂾂낤���ǁA�f�o�b�O�������̂ō��͂�����ł��B
	/// </remarks>
	void BuildTangentAndBiNormal();
private:
	std::vector< SMesh>	m_meshParts;		//���b�V���p�[�c�B
};
