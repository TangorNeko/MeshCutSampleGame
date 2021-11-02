#include "stdafx.h"
#include "Model.h"
#include "Material.h"

void Model::Init(const ModelInitData& initData)
{
	MY_ASSERT(
		initData.m_fxFilePath, 
		"error : initData.m_fxFilePathが指定されていません。"
	);
	MY_ASSERT(
		initData.m_tkmFilePath,
		"error : initData.m_tkmFilePathが指定されていません。"
	);
	//内部のシェーダーをロードする処理が求めているのが
	//wchar_t型の文字列なので、ここで変換しておく。
	wchar_t wfxFilePath[256] = {L""};
	if (initData.m_fxFilePath != nullptr) {
		//MessageBoxA(nullptr, "fxファイルパスが指定されていません。", "エラー", MB_OK);
		//std::abort();
		mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);
	}
	
	if (initData.m_skeleton != nullptr) {
		//スケルトンが指定されている。
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
		//todo マテリアル名をtkmファイルに出力したなかった・・・。
		//todo 今は全マテリアル差し替えます
		for (Material* material : mesh.m_materials) {
			material->GetAlbedoMap().InitFromD3DResource(albedoMap.Get());
		}
	});
	//ディスクリプタヒープの再作成。
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
	//引数の切断面と向きと切断面の一点の座標はワールド座標、しかしメッシュカットはモデルのローカル座標系を基準に行われる(おそらく)ため、
	//モデルのワールド座標の逆行列をかけるとモデルのローカル座標系に変換できるのではないか?

	//法線の向きから、法線の先端のある座標を計算する。
	Vector3 modelLocalCutNormal = worldCutNormal + worldCutPoint;

	//切断面上の一点を格納
	Vector3 modelLocalCutPoint = worldCutPoint;

	//モデルのワールド行列の逆行列を計算
	Matrix invWorld = m_world;
	invWorld.Inverse();

	//ワールド行列の逆行列を各要素に乗算し、モデルのローカル座標に変換
	invWorld.Apply(modelLocalCutNormal);
	invWorld.Apply(modelLocalCutPoint);

	//ローカル座標での法線の先端-ローカル座標の切断面上の一点でローカル座標での法線が求まる?
	modelLocalCutNormal -= modelLocalCutPoint;

	//念の為正規化
	modelLocalCutNormal.Normalize();

	//TODO:分割前に面がモデル全体の頂点を1つでも分割するかの判定?

	Model* newmodel = new Model;

	//分割
	newmodel->m_tkmFile.SetMeshParts(m_tkmFile.Divide(modelLocalCutNormal, modelLocalCutPoint));
	newmodel->TkmFileToMeshParts(initData);
	newmodel->SetWorldMatrix(m_world);

	wchar_t wfxFilePath[256] = { L"" };
	if (initData.m_fxFilePath != nullptr) {
		//MessageBoxA(nullptr, "fxファイルパスが指定されていません。", "エラー", MB_OK);
		//std::abort();
		mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);
	}

	if (initData.m_skeleton != nullptr) {
		//スケルトンが指定されている。
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
		//MessageBoxA(nullptr, "fxファイルパスが指定されていません。", "エラー", MB_OK);
		//std::abort();
		mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);
	}

	if (initData.m_skeleton != nullptr) {
		//スケルトンが指定されている。
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
		//MessageBoxA(nullptr, "fxファイルパスが指定されていません。", "エラー", MB_OK);
		//std::abort();
		mbstowcs(wfxFilePath, initData.m_fxFilePath, 256);
	}

	if (initData.m_skeleton != nullptr) {
		//スケルトンが指定されている。
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
