#pragma once

namespace Game
{
	class CutDummy : public IGameObject
	{
	public:
		~CutDummy();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* skinModelRender) override;

		/**
		 * @brief スキンモデルを直接登録する
		 * @param modelRender スキンモデルレンダー
		*/
		void SetSkinModel(SkinModelRender* modelRender)
		{
			m_dummyModel = modelRender;
		}

		/**
		 * @brief ダミーに使用するカプセルの半径を登録
		 * @param radius 
		*/
		void SetCapsuleRadius(float radius)
		{
			m_dummyRadius = radius;
		}

		//ダミーに使用するカプセルの高さを登録
		void SetCapsuleHeight(float height)
		{
			m_dummyHeight = height;
		}

		/**
		 * @brief 上方向の軸からカプセルの軸への回転クォータニオンと、その回転を戻すクォータニオンをセットする
		 * @param capsuleRot カプセルの軸方向に回転させるクォータニオン
		 * @param toModelRot カプセルの軸方向から上方向の軸に戻すクォータニオン
		*/
		void SetRotations(const Quaternion& capsuleRot, const Quaternion& toModelRot)
		{
			m_capsuleRot = capsuleRot;
			m_toModelRot = toModelRot;
		}
	private:
		SkinModelRender* m_dummyModel = nullptr;			//ダミー用のモデル
		CapsuleCollider m_capsuleCollider;					//カプセル状のコライダー
		float m_dummyRadius = 1.0f;							//ダミーのカプセルの半径
		float m_dummyHeight = 1.0f;							//ダミーのカプセルの高さ
		RigidBody m_rigidBody;								//ダミーの剛体
		Quaternion m_capsuleRot = Quaternion::Identity;		//上方向からカプセルの軸方向へ回転させるクォータニオン
		Quaternion m_toModelRot = Quaternion::Identity;		//カプセルの軸方向から上方向へ回転させる(カプセル軸への回転を戻す)クォータニオン
		Quaternion m_modelRot = Quaternion::Identity;		//元々のモデルのクォータニオン
		bool m_isCut = false;								//切断された?
	};
}

