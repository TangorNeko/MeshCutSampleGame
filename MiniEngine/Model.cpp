#include "stdafx.h"
#include "Model.h"
#include "Material.h"

void Model::Init(const ModelInitData& initData)
{
	MY_ASSERT(
		initData.m_fxFilePath, 
		"error : initData.m_fxFilePath���w�肳��Ă��܂���B"
	);
	MY_ASSERT(
		initData.m_tkmFilePath,
		"error : initData.m_tkmFilePath���w�肳��Ă��܂���B"
	);
	//�����̃V�F�[�_�[�����[�h���鏈�������߂Ă���̂�
	//wchar_t�^�̕�����Ȃ̂ŁA�����ŕϊ����Ă����B
	wchar_t wfxFilePath[256] = {L""};
	if (initData.m_fxFilePath != nullptr) {
		//MessageBoxA(nullptr, "fx�t�@�C���p�X���w�肳��Ă��܂���B", "�G���[", MB_OK);
		//std::abort();
		mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);
	}
	
	if (initData.m_skeleton != nullptr) {
		//�X�P���g�����w�肳��Ă���B
		m_meshParts.BindSkeleton(*initData.m_skeleton);
	}
	
	m_modelUpAxis = initData.m_modelUpAxis;

	m_tkmFile.Load(initData.m_tkmFilePath);
	m_meshParts.InitFromTkmFile(
		m_tkmFile, 
		wfxFilePath, 
		initData.m_vsEntryPointFunc,
		initData.m_vsSkinEntryPointFunc,
		initData.m_psEntryPointFunc,
		initData.m_expandConstantBuffer,
		initData.m_expandConstantBufferSize,
		initData.m_expandShaderResoruceView,
		initData.m_colorBufferFormat
	);

	UpdateWorldMatrix(g_vec3Zero, g_quatIdentity, g_vec3One);
	
}

void Model::UpdateWorldMatrix(Vector3 pos, Quaternion rot, Vector3 scale)
{
	Matrix mBias;
	if (m_modelUpAxis == enModelUpAxisZ) {
		//Z-up
		mBias.MakeRotationX(Math::PI * -0.5f);
	}
	Matrix mTrans, mRot, mScale;
	mTrans.MakeTranslation(pos);
	mRot.MakeRotationFromQuaternion(rot);
	mScale.MakeScaling(scale);
	m_world = mBias * mScale * mRot * mTrans;
}

void Model::ChangeAlbedoMap(const char* materialName, Texture& albedoMap)
{
	m_meshParts.QueryMeshs([&](const SMesh& mesh) {
		//todo �}�e���A������tkm�t�@�C���ɏo�͂����Ȃ������E�E�E�B
		//todo ���͑S�}�e���A�������ւ��܂�
		for (Material* material : mesh.m_materials) {
			material->GetAlbedoMap().InitFromD3DResource(albedoMap.Get());
		}
	});
	//�f�B�X�N���v�^�q�[�v�̍č쐬�B
	m_meshParts.CreateDescriptorHeaps();
	
}
void Model::Draw(RenderContext& rc)
{
	m_meshParts.Draw(
		rc, 
		m_world, 
		g_camera3D->GetViewMatrix(), 
		g_camera3D->GetProjectionMatrix()
	);
}

Model* Model::Divide(const ModelInitData& initData, const Vector3& worldCutNormal, const Vector3& worldCutPoint)
{
	//�����̐ؒf�ʂƌ����Ɛؒf�ʂ̈�_�̍��W�̓��[���h���W�A���������b�V���J�b�g�̓��f���̃��[�J�����W�n����ɍs����(�����炭)���߁A
	//���f���̃��[���h���W�̋t�s���������ƃ��f���̃��[�J�����W�n�ɕϊ��ł���̂ł͂Ȃ���?

	//�@���̌�������A�@���̐�[�̂�����W���v�Z����B
	Vector3 modelLocalCutNormal = worldCutNormal + worldCutPoint;

	//�ؒf�ʏ�̈�_���i�[
	Vector3 modelLocalCutPoint = worldCutPoint;

	//���f���̃��[���h�s��̋t�s����v�Z
	Matrix invWorld = m_world;
	invWorld.Inverse();

	//���[���h�s��̋t�s����e�v�f�ɏ�Z���A���f���̃��[�J�����W�ɕϊ�
	invWorld.Apply(modelLocalCutNormal);
	invWorld.Apply(modelLocalCutPoint);

	//���[�J�����W�ł̖@���̐�[-���[�J�����W�̐ؒf�ʏ�̈�_�Ń��[�J�����W�ł̖@�������܂�?
	modelLocalCutNormal -= modelLocalCutPoint;

	//�O�̈א��K��
	modelLocalCutNormal.Normalize();

	//TODO:�����O�ɖʂ����f���S�̂̒��_��1�ł��������邩�̔���?

	Model* newmodel = new Model;

	//����
	newmodel->m_tkmFile.SetMeshParts(m_tkmFile.Divide(modelLocalCutNormal, modelLocalCutPoint));
	newmodel->TkmFileToMeshParts(initData);
	newmodel->SetWorldMatrix(m_world);

	wchar_t wfxFilePath[256] = { L"" };
	if (initData.m_fxFilePath != nullptr) {
		//MessageBoxA(nullptr, "fx�t�@�C���p�X���w�肳��Ă��܂���B", "�G���[", MB_OK);
		//std::abort();
		mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);
	}

	if (initData.m_skeleton != nullptr) {
		//�X�P���g�����w�肳��Ă���B
		m_meshParts.BindSkeleton(*initData.m_skeleton);
	}

	m_modelUpAxis = initData.m_modelUpAxis;

	m_meshParts.InitFromTkmFile(
		m_tkmFile,
		wfxFilePath,
		initData.m_vsEntryPointFunc,
		initData.m_vsSkinEntryPointFunc,
		initData.m_psEntryPointFunc,
		initData.m_expandConstantBuffer,
		initData.m_expandConstantBufferSize,
		initData.m_expandShaderResoruceView,
		initData.m_colorBufferFormat
	);

	return newmodel;
}

void Model::TkmFileToMeshParts(const ModelInitData& initData)
{
	wchar_t wfxFilePath[256] = { L"" };
	if (initData.m_fxFilePath != nullptr) {
		//MessageBoxA(nullptr, "fx�t�@�C���p�X���w�肳��Ă��܂���B", "�G���[", MB_OK);
		//std::abort();
		mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);
	}

	if (initData.m_skeleton != nullptr) {
		//�X�P���g�����w�肳��Ă���B
		m_meshParts.BindSkeleton(*initData.m_skeleton);
	}

	m_modelUpAxis = initData.m_modelUpAxis;

	m_meshParts.InitFromTkmFile(
		m_tkmFile,
		wfxFilePath,
		initData.m_vsEntryPointFunc,
		initData.m_vsSkinEntryPointFunc,
		initData.m_psEntryPointFunc,
		initData.m_expandConstantBuffer,
		initData.m_expandConstantBufferSize,
		initData.m_expandShaderResoruceView,
		initData.m_colorBufferFormat
	);
}

Vector4 Model::GetOriginToCenter()
{
	return m_tkmFile.GetOriginToCenter();
}

void Model::SetOriginOffset(const Vector3& offset, const ModelInitData& initData)
{
	m_tkmFile.SetOriginOffset(offset);

	wchar_t wfxFilePath[256] = { L"" };
	if (initData.m_fxFilePath != nullptr) {
		//MessageBoxA(nullptr, "fx�t�@�C���p�X���w�肳��Ă��܂���B", "�G���[", MB_OK);
		//std::abort();
		mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);
	}

	if (initData.m_skeleton != nullptr) {
		//�X�P���g�����w�肳��Ă���B
		m_meshParts.BindSkeleton(*initData.m_skeleton);
	}

	m_meshParts.InitFromTkmFile(
		m_tkmFile,
		wfxFilePath,
		initData.m_vsEntryPointFunc,
		initData.m_vsSkinEntryPointFunc,
		initData.m_psEntryPointFunc,
		initData.m_expandConstantBuffer,
		initData.m_expandConstantBufferSize,
		initData.m_expandShaderResoruceView,
		initData.m_colorBufferFormat
	);
}
