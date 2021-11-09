#include "stdafx.h"
#include "SkinModelRender.h"
#include "CutDummy.h"

namespace
{
	const int MODE_MAX_DIVIDE_NUM = 4;
}

namespace Game
{
	void SkinModelRender::Render(RenderContext& rc)
	{
		m_model->Draw(rc);
	}

	void SkinModelRender::Init(const char* modelPath, const char* skeletonPath, AnimationClip* animClips, int animClipNum, EnModelUpAxis modelUpAxis)
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
		m_modelInitData.m_modelUpAxis = modelUpAxis;

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

	void SkinModelRender::Init(const char* modelPath, const char* skeletonPath, EnModelUpAxis modelUpAxis)
	{
		Init(modelPath, skeletonPath, nullptr, 0, modelUpAxis);
	}

	void SkinModelRender::Init(const char* modelPath, EnModelUpAxis modelUpAxis)
	{
		Init(modelPath, nullptr, nullptr, 0, modelUpAxis);
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

	void SkinModelRender::Divide(const Vector3& cutNormal, const Vector3& cutPoint)
	{
		if (m_isDividable == true && m_divideNum <= MODE_MAX_DIVIDE_NUM)
		{
			//モデルを分割し、表面側のモデルを自らに格納、裏面側のモデルをポインタとして取得
			Model* backModel = m_model->Divide(m_modelInitData, cutNormal, cutPoint);

			if (backModel == nullptr)
			{
				return;
			}

			//完全に1つもメッシュ、マテリアルが残らなかった時の処理
			if (backModel->GetTkmFile().GetNumMesh() == 0)
			{
				delete backModel;
				return;
			}
			
			//分断されたので切断回数をプラス
			m_divideNum++;

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

			//分割された回数を共有
			backModelRender->SetDivideNum(m_divideNum);

			//物理エンジンを利用したダミーの作成
			//NOTE:カットされるモデルの本体はずっと物理エンジンを使用しない形で残ります。

			//切り離されたモデルの原点をAABBからモデルの中心に設定し、中心からAABBまでの一点までの距離を取得
			backModelRender->SetModelCenterAsOrigin();

			//カプセルのデータを取得
			Vector2 heightAndRadius;
			Vector3 capsuleAxis = backModelRender->CalcCapsuleData(heightAndRadius);

			CutDummy* dummy = NewGO<CutDummy>(0);
			dummy->SetSkinModel(backModelRender);

			//カプセルのデータをセット
			//TODO:回転も適用
			//heightAndRadiusのxに高さ、yに半径が入っている
			dummy->SetCapsuleHeight(heightAndRadius.x);
			dummy->SetCapsuleRadius(heightAndRadius.y);

			//カプセルの軸がローカル座標系なのでワールド座標系に変換
			Matrix modelWorldMatrix = backModel->GetWorldMatrix();
			modelWorldMatrix.Apply(capsuleAxis);
			Vector3 localOrigin = Vector3::Zero;
			modelWorldMatrix.Apply(localOrigin);
			capsuleAxis -= localOrigin;
			capsuleAxis.Normalize();

			//カプセルは最初に軸が傾いている分回転させるが、そのままだとモデルもその回転についてくるので、
			//戻すクォータニオンも求めておく
			Quaternion capsuleRot,toModelRot;
			//カプセルの傾きのクォータニオン
			capsuleRot.SetRotation(Vector3::Up, capsuleAxis);
			//カプセルの傾きを戻す(モデルに乗算する)クォータニオン
			toModelRot.SetRotation(capsuleAxis, Vector3::Up);

			dummy->SetRotations(capsuleRot, toModelRot);

			//カット可能なモデル一覧に追加
			ModelCutManager::GetInstance()->AddNextCuttable(backModelRender);

			if (m_owner != nullptr)
			{
				m_owner->OnDivide();
			}
		}
	}

	void SkinModelRender::SetModelCenterAsOrigin()
	{
		Vector4 originToCenter = m_model->GetOriginToCenter();
		Vector3 OriginOffset = { originToCenter.x,originToCenter.y ,originToCenter.z };
		m_model->SetOriginOffset(OriginOffset, m_modelInitData);

		Vector3 worldOrigin = OriginOffset;

		//NOTE:モデルが拡大されている場合wに入っている距離も拡大されるのではないか?その場合はどう計算したらいいだろうか。
		m_model->GetWorldMatrix().Apply(worldOrigin);
		SetPosition(worldOrigin);
	}
}