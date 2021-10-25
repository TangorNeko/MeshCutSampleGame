#pragma once

namespace Game
{
	class PlayerCamera
	{
	public:
		void Update(const Vector3& playerPosition);
	private:
		Vector3 m_cameraTarget = Vector3::Zero;
		Vector3 m_toCameraDirection = Vector3::AxisZ ;
	};
}

