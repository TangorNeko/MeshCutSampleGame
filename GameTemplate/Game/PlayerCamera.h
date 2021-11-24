#pragma once

namespace Game
{
	class PlayerCamera
	{
	public:
		void Update(const Vector3& playerPosition);

		void UpdateCutMode(const Vector3& playerPosition);
	private:
		Vector3 m_cameraTarget = Vector3::Zero;			//カメラの注視点
		Vector3 m_toCameraDirection = Vector3::AxisZ;	//カメラの注視点からカメラの座標への向き
	};
}

