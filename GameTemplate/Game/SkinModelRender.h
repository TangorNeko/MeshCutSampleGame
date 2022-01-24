#pragma once

namespace Game
{
	class SkinModelRender : public IGameObject
	{
	private:
		~SkinModelRender()
		{
			//切断可能リストから削除
			ModelCutManager::GetInstance()->RemoveCuttable(this);

			//モデルが存在していたら削除
			if (m_model != nullptr)
			{
				delete m_model;
			}

			if (m_shadowModel != nullptr)
			{
				delete m_shadowModel;
			}
		}

		/**
		 * @brief モデルの座標、回転、拡大のアップデート
		*/
		void UpdateModel()
		{
			m_model->UpdateWorldMatrix(m_position, m_qRot, m_scale);

			if (m_shadowModel != nullptr)
			{
				m_shadowModel->UpdateWorldMatrix(m_position, m_qRot, m_scale);
			}
			m_skeleton.Update(m_model->GetWorldMatrix());
		}

	public:
		/**
		 * @brief モデルの描画
		 * @param rc レンダーコンテキスト
		*/
		void Render(RenderContext& rc)override;

		/**
		 * @brief モデルの初期化関数　アニメーションつき
		 * @param modelPath モデルファイルのパス(.tkm)
		 * @param skeletonPath スケルトンファイルのパス(.tks)
		 * @param animClips アニメーションクリップの配列
		 * @param animClipNum アニメーションクリップの数
		*/
		void Init(const char* modelPath, const char* skeletonPath, AnimationClip* animClips, int animClipNum, EnModelUpAxis modelUpAxis = enModelUpAxisY);

		/**
		 * @brief モデルの初期化関数
		 * @param modelPath モデルファイルのパス(.tkm)
		 * @param skeletonPath スケルトンファイルのパス(.tks)
		*/
		void Init(const char* modelPath, const char* skeletonPath, EnModelUpAxis modelUpAxis = enModelUpAxisZ);

		/**
		 * @brief モデルの初期化関数　モデルパスだけ版
		 * @param modelPath モデルファイルのパス(.tkm)
		*/
		void Init(const char* modelPath, EnModelUpAxis modelUpAxis = enModelUpAxisZ);

		/**
		 * @brief ライティングなしでモデルを初期化
		 * @param modelPath モデルファイルのパス(.tkm)
		 * @param modelUpAxis モデルの上方向の軸
		*/
		void InitUnlit(const char* modelPath, EnModelUpAxis modelUpAxis = enModelUpAxisZ);

		/**
		 * @brief モデルクラスのポインタからの初期化
		 * @param model 
		*/
		void InitFromModel(Model* model);

		/**
		 * @brief アニメーションの再生
		 * @param animationNo アニメーション番号
		 * @param interpolateTime アニメーションの補完時間
		*/
		void PlayAnimation(int animationNo, float interpolateTime = 0.0f)
		{
			m_animation.Play(animationNo, interpolateTime);
			m_animation.Progress(m_animationSpeed / 60.0f);
			UpdateModel();
		}

		/**
		 * @brief アニメーションの再生速度を設定する
		 * @param animationSpeed 再生速度
		*/
		void SetAnimationSpeed(float animationSpeed)
		{
			m_animationSpeed = animationSpeed;
		}

		/**
		 * @brief アニメーションを再生しているか?
		 * @return 再生している:true 再生していない:false
		*/
		bool IsPlayingAnimation() const
		{
			return m_animation.IsPlaying();
		}

		/**
		 * @brief モデルの座標を設定する
		 * @param pos 座標
		*/
		void SetPosition(Vector3 pos)
		{
			m_position = pos;
			UpdateModel();
		}

		/**
		 * @brief モデルの回転を設定する
		 * @param qRot 回転
		*/
		void SetRotation(Quaternion qRot)
		{
			m_qRot = qRot;
			UpdateModel();
		}

		/**
		 * @brief モデルの拡大率を設定する
		 * @param scale 拡大率
		*/
		void SetScale(Vector3 scale)
		{
			m_scale = scale;
			UpdateModel();
		}

		/**
		 * @brief モデルと線分が交差している?
		 * @param start 線分の始点
		 * @param end 線分の終点
		 * @param crossPoint 線分の交差点(戻り値、参照受け取り)
		 * @return 交差している:true 交差していない:false
		*/
		bool isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint);

		/**
		 * @brief モデルのワールド行列を直接セットする
		 * @param world ワールド行列
		*/
		void SetMatrix(Matrix world)
		{
			m_model->SetWorldMatrix(world);
			m_skeleton.Update(world);
		}

		/**
		 * @brief 切断可能かのフラグをセット
		 * @param isDividable 切断可能か?
		*/
		void SetDivideFlag(bool isDividable);

		/**
		 * @brief モデルの初期化データをセット
		 * @param modelInitData モデルの初期化データ
		*/
		void SetModelInitData(const ModelInitData& modelInitData)
		{
			//TODO:modelInitDataを使わない方法を探す
			m_modelInitData = modelInitData;
		}

		/**
		 * @brief モデルの座標を取得
		 * @return 座標
		*/
		const Vector3& GetPosition() const
		{
			return m_position;
		}

		/**
		 * @brief モデルの回転を取得
		 * @return 回転
		*/
		const Quaternion& GetRotation() const
		{
			return m_qRot;
		}

		/**
		 * @brief モデルの拡大率を取得
		 * @return 拡大率
		*/
		const Vector3& GetScale() const
		{
			return m_scale;
		}

		/**
		 * @brief 切断可能かのフラグを取得
		 * @return 切断可能か?
		*/
		bool GetDivideFlag() const
		{
			return m_isDividable;
		}

		/**
		 * @brief モデルを取得
		 * @return モデル
		*/
		Model* GetModel()
		{
			return m_model;
		}

		/**
		 * @brief モデルの切断
		 * @param cutNormal 切断面の一点からの法線の向き
		 * @param cutPoint 切断面の一点の座標
		 * @param cutForce 切断時にかかる力
		*/
		void Divide(const Vector3& cutNormal, const Vector3& cutPoint, const Vector3& cutForce);

		/**
		 * @brief モデルの切断
		 * @param cutNormal 切断面の一点からの法線の向き
		 * @param cutPoint 切断面の一点の座標
		*/
		void Divide(const Vector3& cutNormal, const Vector3& cutPoint)
		{
			Divide(cutNormal, cutPoint, Vector3::Zero);
		}

		/**
		 * @brief モデルの原点をモデルのAABBの中心に合わせる
		 * @return モデルのAABBの一点から中心への距離
		*/
		void SetModelCenterAsOrigin();

		/**
		 * @brief 切断された回数をセット
		 * @param divideNum 切断された回数
		*/
		void SetDivideNum(int divideNum)
		{
			m_divideNum = divideNum;
		}

		/**
		 * @brief 切断された回数を取得
		 * @return 
		*/
		int GetDivideNum() const
		{
			return m_divideNum;
		}

		/**
		 * @brief モデルを元にカプセルの縦方向の軸、高さ、幅を計算する
		 * @param heightAndRadius 高さ、幅の受け取り用Vector2クラス(xに高さ、yに幅を格納する)
		 * @return 
		*/
		Vector3 CalcCapsuleData(Vector2& heightAndRadius)
		{
			return m_model->CalcCapsuleData(heightAndRadius);
		}

		/**
		 * @brief 自らのモデルから物理演算するダミーを作成する
		 * @param cutForce 最初にダミーにかかる力
		*/
		void MakeDummy(const Vector3& cutForce);

		/**
		 * @brief スキンモデルの所有者を登録
		 * @param gameObject 所有者のポインタ
		*/
		void SetOwner(IGameObject* gameObject)
		{
			m_owner = gameObject;
		}

		/**
		 * @brief モデルの原点からコアまでのベクトルを設定、切断時にコア座標のある側のモデルが優先して残される(コア座標のない側がダミーになる)
		 * @param toCoreVec モデルの原点からコアまでのベクトル
		*/
		void SetToCoreVector(const Vector3& toCoreVec)
		{
			m_toCoreVector = toCoreVec;
		}

		/**
		 * @brief モデルの原点からコアまでのベクトルを取得
		 * @return モデルの原点からコアまでのベクトル
		*/
		Vector3 GetToCoreVector()
		{
			return m_toCoreVector;
		}

		/**
		 * @brief コアの座標が切断平面の表側にあるか?
		 * @param cutNormal 切断面の一点からの法線の向き
		 * @param cutPoint 切断面の一点の座標
		 * @return 
		*/
		bool IsCoreOnFront(const Vector3& cutNormal, const Vector3& cutPoint);

		/**
		 * @brief 影用モデルの再構成
		*/
		void ShadowModelReconstruction();
	private:
		IGameObject* m_owner = nullptr;				//スキンモデルの所有者
		Model* m_model = nullptr;					//モデル
		Model* m_shadowModel = nullptr;				//影用のモデル
		Vector3 m_position = Vector3::Zero;			//座標
		Quaternion m_qRot = Quaternion::Identity;	//回転
		Vector3 m_scale = Vector3::One;				//拡大
		Skeleton m_skeleton;						//スケルトン
		AnimationClip* m_animationClips = nullptr;	//アニメーションクリップ
		int m_animationClipNum = 0;					//アニメーションクリップの数
		Animation m_animation;						//アニメーション
		float m_animationSpeed = 1.0f;				//アニメーションの速度
		bool m_isDividable = false;					//切断可能?
		int m_divideNum = 0;						//切断された回数
		Vector3 m_toCoreVector = Vector3::Zero;	//モデルの原点から切断した時に残したいコア部分へのベクトル

		//NOTE:切断して新しくできたモデルを初期化するためには元のモデルのModelInitDataが
		//必要なので、SkinModelRenderのメンバ変数として入れている
		//TODO:なんとかして別の方法を探したいが...
		ModelInitData m_modelInitData;				//モデルの初期化データ
	};
}

