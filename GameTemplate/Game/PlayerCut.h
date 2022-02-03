#pragma once

namespace Game
{
	class PlayerCut
	{
	public:
		~PlayerCut();

		/**
		 * @brief プレイヤーの切断処理の更新
		 * @param playerPosition プレイヤーの座標
		 * @param playerQRot プレイヤーの回転
		*/
		void Update(const Vector3& playerPosition,const Quaternion& playerQRot);

		/**
		 * @brief 切断モード中の更新
		 * @param playerPosition プレイヤーの座標
		 * @param playerQRot プレイヤーの回転
		*/
		void CutUpdate(const Vector3& playerPosition,const Quaternion& playerQRot);

		/**
		 * @brief 非切断モード中の更新
		 * @param playerPosition プレイヤーの座標
		 * @param playerQRot プレイヤーの回転
		*/
		void NormalUpdate(const Vector3& playerPosition,const Quaternion& playerQRot);

		/**
		 * @brief 切断面のモデルを取得
		 * @return 
		*/
		SkinModelRender* GetModel()
		{
			return m_cutPlaneRender;
		}

		void SetCutEvent(bool flag, const Vector3& playerPosition);
	private:
		SkinModelRender* m_cutPlaneRender = nullptr;		//切断面を示すモデル
		bool m_isCutMode = false;							//切断モード中?
		bool m_isCutEvent = false;							//イベント切断モード中?
		bool m_isStart = true;								//最初の切断モード?
		Quaternion m_cutPlaneQRot = Quaternion::Identity;	//切断面を表すモデルの回転
		float angle = 0.0f;									//切断の角度
	};
}

