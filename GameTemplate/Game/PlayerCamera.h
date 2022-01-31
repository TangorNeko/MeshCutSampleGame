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
		Vector3 m_cameraPosition = Vector3::Zero;		//�J�����̍��W
		Vector3 m_cameraTarget = Vector3::Zero;			//�J�����̒����_
		Vector3 m_cutCameraPosition = Vector3::Zero;	//�ؒf���[�h���̃J�����̍��W
		Vector3 m_cutCameraTarget = Vector3::Zero;		//�ؒf���[�h���̃J�����̒����_
		Vector3 m_toCameraDirection = Vector3::AxisZ;	//�J�����̒����_����J�����̍��W�ւ̌���
		float m_totalDegX = 0.0f;						//�J������X���p�x�̍��v
		float m_lerpValue = 0.0f;
		BackGround* m_backGround = nullptr;
	};
}

