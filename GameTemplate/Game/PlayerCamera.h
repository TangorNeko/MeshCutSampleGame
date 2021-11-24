#pragma once

namespace Game
{
	class PlayerCamera
	{
	public:
		void Update(const Vector3& playerPosition);

		void UpdateCutMode(const Vector3& playerPosition);
	private:
		Vector3 m_cameraTarget = Vector3::Zero;			//�J�����̒����_
		Vector3 m_toCameraDirection = Vector3::AxisZ;	//�J�����̒����_����J�����̍��W�ւ̌���
	};
}

