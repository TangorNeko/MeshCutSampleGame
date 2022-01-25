#include "stdafx.h"
#include "SkinModelRender.h"
#include "CutDummy.h"

namespace
{
	const int MODE_MAX_DIVIDE_NUM = 3;
}

namespace Game
{
	void SkinModelRender::Render(RenderContext& rc,Camera* camera)
	{
		switch (rc.GetRenderStep())
		{
		case RenderContext::enModel:
			m_model->Draw(rc,camera);
			break;
		case RenderContext::enShadow:
			if (m_shadowModel != nullptr && m_shadowCasterFlag == true)
			{
				m_shadowModel->Draw(rc, camera);
			}
		}
	}

	void SkinModelRender::Init(const char* modelPath, const char* skeletonPath, AnimationClip* animClips, int animClipNum, EnModelUpAxis modelUpAxis)
	{
		//モデルがなければ生成
		if (m_model == nullptr)
		{
			m_model = new Model;
		}

		if (m_shadowModel == nullptr)
		{
			m_shadowModel = new Model;
		}

		//モデルのファイルパスの指定
		m_modelInitData.m_tkmFilePath = modelPath;

		//シェーダーパスの指定
		m_modelInitData.m_fxFilePath = "Assets/shader/shadowReceiver.fx";

		//シェーダーの頂点シェーダーのエントリー関数名の指定
		m_modelInitData.m_vsEntryPointFunc = "VSMain";

		//シェーダーのスキンあり頂点シェーダーのエントリー関数名の指定
		m_modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";

		//スケルトンが存在している時はスケルトンを初期化
		if (skeletonPath != nullptr)
		{
			m_skeleton.Init(skeletonPath);
			m_modelInitData.m_skeleton = &m_skeleton;
		}

		//モデルデータの上方向の軸を指定
		m_modelInitData.m_modelUpAxis = modelUpAxis;

		//ここからは影用のModelInitDataと通常モデル用のModelInitDataが違うので2つに分ける。
		ModelInitData shadowModelInitData = m_modelInitData;

		m_modelInitData.m_expandShaderResoruceView = &Graphics::g_renderingEngine->GetShadowMap();

		m_modelInitData.m_expandConstantBufferSize[0] = Light::LightManager::GetInstance()->GetLigDataSize();
		m_modelInitData.m_expandConstantBuffer[0] = Light::LightManager::GetInstance()->GetLigDatas();
		
		m_modelInitData.m_expandConstantBufferSize[1] = Light::LightManager::GetInstance()->GetLigCameraDataSize();
		m_modelInitData.m_expandConstantBuffer[1] = Light::LightManager::GetInstance()->GetLigCameraDatas();

		shadowModelInitData.m_fxFilePath = "Assets/shader/shadow.fx";
		shadowModelInitData.m_colorBufferFormat = DXGI_FORMAT_R32_FLOAT;
		shadowModelInitData.m_expandConstantBufferSize[0] = Light::LightManager::GetInstance()->GetLigCameraDataSize();
		shadowModelInitData.m_expandConstantBuffer[0] = Light::LightManager::GetInstance()->GetLigCameraDatas();

		//モデルの初期化
		m_model->Init(m_modelInitData);
		m_shadowModel->Init(shadowModelInitData);

		//アニメーション関連の初期化
		m_animationClips = animClips;
		m_animationClipNum = animClipNum;

		if (m_animationClips != nullptr)
		{
			m_animation.Init(m_skeleton, m_animationClips, m_animationClipNum);
		}
	}

	void SkinModelRender::Init(const char* modelPath, const char* skeletonPath, EnModelUpAxis modelUpAxis)
	{
		Init(modelPath, skeletonPath, nullptr, 0, modelUpAxis);
	}

	void SkinModelRender::Init(const char* modelPath, EnModelUpAxis modelUpAxis)
	{
		Init(modelPath, nullptr, nullptr, 0, modelUpAxis);
	}

	void SkinModelRender::InitUnlit(const char* modelPath, EnModelUpAxis modelUpAxis)
	{
		//モデルがなければ生成
		if (m_model == nullptr)
		{
			m_model = new Model;
		}

		//モデルのファイルパスの指定
		m_modelInitData.m_tkmFilePath = modelPath;

		//シェーダーパスの指定
		m_modelInitData.m_fxFilePath = "Assets/shader/unlit.fx";

		//シェーダーの頂点シェーダーのエントリー関数名の指定
		m_modelInitData.m_vsEntryPointFunc = "VSMain";

		//シェーダーのピクセルシェーダーのエントリー関数名の指定
		m_modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";

		//モデルデータの上方向の軸を指定
		m_modelInitData.m_modelUpAxis = modelUpAxis;

		//モデルの初期化
		m_model->Init(m_modelInitData);
	}

	bool SkinModelRender::isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint)
	{
		return m_model->isLineHitModel(start, end, crossPoint);
	}

	void SkinModelRender::InitFromModel(Model* model)
	{
		if (m_model == nullptr)
		{
			m_model = model;
		}
	}

	void SkinModelRender::SetDivideFlag(bool isDividable)
	{
		m_isDividable = isDividable;

		if (m_isDividable == false)
		{
			//カット可能モデルから削除
			ModelCutManager::GetInstance()->RemoveCuttable(this);
		}
	}

	void SkinModelRender::Divide(const Vector3& cutNormal, const Vector3& cutPoint, const Vector3& cutForce)
	{
		//切断不可な状態なら何もしない。
		if (m_isDividable == false || m_divideNum > MODE_MAX_DIVIDE_NUM)
		{
			return;
		}

		Vector3 coreRemainCutNormal = cutNormal;
		//コアの座標を残すように判定し必要なら法線を反転する
		if (IsCoreOnFront(coreRemainCutNormal, cutPoint) == false)
		{
			coreRemainCutNormal *= -1.0f;
		}

		//モデルを分割し、表面側のモデルを自らに格納、裏面側のモデルをポインタとして取得
		Model* backModel = m_model->Divide(m_modelInitData, coreRemainCutNormal, cutPoint);

		//切断判定で切断されないと判断された場合何もしない
		if (backModel == nullptr)
		{
			return;
		}

		//完全に1つもメッシュ、マテリアルが残らなかった場合の処理
		//NOTE:切断されないと判断された場合以外に完全にメッシュ、マテリアルが残らない場合があるかは不明
		if (backModel->GetTkmFile().GetNumMesh() == 0)
		{
			delete backModel;
			return;
		}
		
		//分断されたので切断回数をプラス
		m_divideNum++;

		//表側(元のモデル)の影用モデルを再構成
		ShadowModelReconstruction();

		//裏面側のモデルを描画するモデルレンダークラスを作成
		SkinModelRender* backModelRender = NewGO<SkinModelRender>(0);

		//新しく作ったモデルの初期化
		//TODO:まとめる
		backModelRender->InitFromModel(backModel);
		backModelRender->SetModelInitData(m_modelInitData);

		//裏側(新しくできたモデル)の影用モデルを構成
		//TODO:厳密には裏側の時は再構成ではない
		backModelRender->ShadowModelReconstruction();

		backModelRender->SetPosition(m_position);
		backModelRender->SetRotation(m_qRot);
		backModelRender->SetScale(m_scale);
		backModelRender->SetDivideFlag(true);

		//分割された回数を共有
		backModelRender->SetDivideNum(m_divideNum);

		//物理エンジンを利用したダミーの作成

		backModelRender->MakeDummy(cutForce);

		//カット可能なモデル一覧に追加
		ModelCutManager::GetInstance()->AddNextCuttable(backModelRender);

		if (m_owner != nullptr)
		{
			m_owner->OnDivide(this,cutForce);
		}
	}

	void SkinModelRender::MakeDummy(const Vector3& cutForce)
	{
		//切り離されたモデルの原点をAABBからモデルの中心に設定し、中心からAABBまでの一点までの距離を取得
		SetModelCenterAsOrigin();

		//カプセルのデータを取得
		Vector2 heightAndRadius;
		Vector3 capsuleAxis = CalcCapsuleData(heightAndRadius);

		CutDummy* dummy = NewGO<CutDummy>(0,"cutDummy");
		dummy->SetSkinModel(this);

		//カプセルのデータをセット
		
		//heightAndRadiusのxに高さ、yに半径が入っている
		//NOTE:切断されるモデルの拡大率はxyzすべて同一でないとそもそもうまく斬れないので、
		//いずれかの拡大率を乗算すればモデルの大きさに合ったカプセルの大きさになる
		dummy->SetCapsuleHeight(heightAndRadius.x * m_scale.x);
		dummy->SetCapsuleRadius(heightAndRadius.y * m_scale.x);

		//カプセルの軸がローカル座標系なのでワールド座標系に変換
		Matrix modelWorldMatrix = m_model->GetWorldMatrix();
		modelWorldMatrix.Apply(capsuleAxis);
		Vector3 localOrigin = Vector3::Zero;
		modelWorldMatrix.Apply(localOrigin);
		capsuleAxis -= localOrigin;
		capsuleAxis.Normalize();

		//カプセルは最初に軸が傾いている分回転させるが、そのままだとモデルもその回転についてくるので、
		//戻すクォータニオンも求めておく
		Quaternion capsuleRot, toModelRot;
		//カプセルの傾きのクォータニオン
		capsuleRot.SetRotation(Vector3::Up, capsuleAxis);
		//カプセルの傾きを戻す(モデルに乗算する)クォータニオン
		toModelRot.SetRotation(capsuleAxis, Vector3::Up);

		dummy->SetRotations(capsuleRot, toModelRot);
		dummy->SetCutForce(cutForce);
	}

	void SkinModelRender::SetModelCenterAsOrigin()
	{
		//モデルの現在の原点からモデルの中心へのベクトルを求める
		Vector3 OriginOffset = m_model->GetOriginToCenter();

		//ベクトル分モデルの原点をずらす
		m_model->SetOriginOffset(OriginOffset, m_modelInitData);
		m_shadowModel->SetOriginOffset(OriginOffset, m_modelInitData);

		Vector3 worldOrigin = OriginOffset;

		//そのままだと表示位置がずれるのでずらした分を打ち消すように座標移動
		m_model->GetWorldMatrix().Apply(worldOrigin);
		SetPosition(worldOrigin);
	}

	bool SkinModelRender::IsCoreOnFront(const Vector3& cutNormal, const Vector3& cutPoint)
	{
		//切断面の一点からコアへのベクトルと法線方向のベクトルを求めて正規化する
		Vector3 corePosition = m_position + m_toCoreVector;
		Vector3 pointToCore = corePosition - cutPoint;
		pointToCore.Normalize();

		//内積を求める
		float dot = Dot(pointToCore, cutNormal);

		//内積の結果が0以上なら法線と同じ方向　つまり表にある
		if (dot >= 0)
		{
			return true;
		}
		else
		{
			//0未満なら法線と違う方向　つまり裏にある
			return false;
		}
	}

	void SkinModelRender::ShadowModelReconstruction()
	{
		//現在の影モデルを削除
		delete m_shadowModel;

		//新しく影モデルを作成
		m_shadowModel = new Model;

		//モデルからコピーしてくる。
		m_model->CopyTo(m_shadowModel);

		//影用のModelInitDataの設定
		ModelInitData shadowModelInitData = m_modelInitData;

		shadowModelInitData.m_fxFilePath = "Assets/shader/shadow.fx";
		shadowModelInitData.m_colorBufferFormat = DXGI_FORMAT_R32_FLOAT;
		shadowModelInitData.m_expandConstantBufferSize[0] = Light::LightManager::GetInstance()->GetLigCameraDataSize();
		shadowModelInitData.m_expandConstantBuffer[0] = Light::LightManager::GetInstance()->GetLigCameraDatas();

		m_shadowModel->TkmFileToMeshParts(shadowModelInitData);
	}
}