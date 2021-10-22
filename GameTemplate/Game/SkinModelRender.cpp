#include "stdafx.h"
#include "SkinModelRender.h"

namespace Game
{
	void SkinModelRender::Render(RenderContext& rc)
	{
		m_model->Draw(rc);
	}

	void SkinModelRender::Init(const char* modelPath, const char* skeletonPath, AnimationClip* animClips, int animClipNum)
	{
		//モデルがなければ生成
		if (m_model == nullptr)
		{
			m_model = new Model;
		}

		//モデルのファイルパスの指定
		m_modelInitData.m_tkmFilePath = modelPath;

		//シェーダーパスの指定
		m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";

		//シェーダーの頂点シェーダーのエントリー関数名の指定
		m_modelInitData.m_vsEntryPointFunc = "VSMain";

		//シェーダーのピクセルシェーダーのエントリー関数名の指定
		m_modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";

		//スケルトンが存在している時はスケルトンを初期化
		if (skeletonPath != nullptr)
		{
			m_skeleton.Init(skeletonPath);
			m_modelInitData.m_skeleton = &m_skeleton;
		}

		//モデルデータの上方向の軸を指定
		m_modelInitData.m_modelUpAxis = enModelUpAxisZ;

		//モデルの初期化
		m_model->Init(m_modelInitData);

		//アニメーション関連の初期化
		m_animationClips = animClips;
		m_animationClipNum = animClipNum;

		if (m_animationClips != nullptr)
		{
			m_animation.Init(m_skeleton, m_animationClips, m_animationClipNum);
		}
	}

	void SkinModelRender::Init(const char* modelPath, const char* skeletonPath)
	{
		Init(modelPath, skeletonPath, nullptr, 0);
	}

	void SkinModelRender::Init(const char* modelPath)
	{
		Init(modelPath, nullptr, nullptr, 0);
	}

	void SkinModelRender::InitFromModel(Model* model)
	{
		if (m_model == nullptr)
		{
			m_model = model;
		}
	}

	void SkinModelRender::Divide(const Vector3& cutNormal, const Vector3& cutPoint)
	{
		if (m_isDividable == true)
		{
			//モデルを分割し、表面側のモデルを自らに格納、裏面側のモデルをポインタとして取得
			Model* backModel = m_model->Divide(m_modelInitData, cutNormal, cutPoint);

			//裏面側のモデルを描画するモデルレンダークラスを作成
			SkinModelRender* backModelRender = NewGO<SkinModelRender>(0);

			//新しく作ったモデルの初期化
			//TODO:まとめる
			backModelRender->InitFromModel(backModel);
			backModelRender->SetPosition(m_position);
			backModelRender->SetRotation(m_qRot);
			backModelRender->SetScale(m_scale);
			backModelRender->SetDivideFlag(true);
			backModelRender->SetModelInitData(m_modelInitData);
		}
	}
}