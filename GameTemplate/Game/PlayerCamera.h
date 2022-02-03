#pragma once
#include "PlayerAnimationParam.h"

namespace Game
{
	class BackGround;
	class PlayerCamera
	{
	public:
		PlayerCamera();

		/**
		 * @brief カメラのアップデート
		 * @param playerPosition プレイヤーの座標
		*/
		void Update(const Vector3& playerPosition);

		/**
		 * @brief 切断モードのカメラのアップデート
		 * @param playerPosition プレイヤーの座標
		 * @param playerDirection プレイヤーの向き
		 * @return 
		*/
		Vector3 UpdateCutMode(const Vector3& playerPosition,const Vector3& playerDirection);

		/**
		 * @brief トドメカメラのアップデート
		 * @param animParam 
		*/
		void UpdateFinishCamera(PlayerAnimationParam& animParam);

		/**
		 * @brief カメラの座標を取得
		 * @return カメラの座標
		*/
		const Vector3& GetCameraPosition()
		{
			return m_cameraPosition;
		}
		
		/**
		 * @brief カメラの注視点を取得
		 * @return 注視点の座標
		*/
		const Vector3& GetCameraTarget()
		{
			return m_cameraTarget;
		}
	private:
		Vector3 m_cameraPosition = Vector3::Zero;		//カメラの座標
		Vector3 m_cameraTarget = Vector3::Zero;			//カメラの注視点
		Vector3 m_cutCameraPosition = Vector3::Zero;	//切断モード時のカメラの座標
		Vector3 m_cutCameraTarget = Vector3::Zero;		//切断モード時のカメラの注視点
		Vector3 m_toCameraDirection = Vector3::AxisZ;	//カメラの注視点からカメラの座標への向き
		float m_totalDegX = 0.0f;						//カメラのX軸角度の合計
		float m_lerpValue = 0.0f;						//カメラの位置の線形補間率
		BackGround* m_backGround = nullptr;				//背景クラスのポインタ(バネカメラ用)
	};
}

