#pragma once
#include "PlayerAnimationParam.h"

namespace Game
{
	class BackGround;
	class PlayerCamera
	{
	public:
		PlayerCamera();

		void Update(const Vector3& playerPosition);

		Vector3 UpdateCutMode(const Vector3& playerPosition,const Vector3& playerDirection);

		void UpdateFinishCamera(PlayerAnimationParam& animParam);

		const Vector3& GetCameraPosition()
		{
			return m_cameraPosition;
		}
		
		const Vector3& GetCameraTarget()
		{
			return m_cameraTarget;
		}
	private:
		Vector3 m_cameraPosition = Vector3::Zero;
		Vector3 m_cameraTarget = Vector3::Zero;			//カメラの注視点
		Vector3 m_toCameraDirection = Vector3::AxisZ;	//カメラの注視点からカメラの座標への向き
		float m_totalDegX = 0.0f;						//カメラのX軸角度の合計
		BackGround* m_backGround = nullptr;
	};
}

