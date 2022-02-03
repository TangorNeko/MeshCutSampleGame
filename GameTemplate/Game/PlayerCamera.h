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
		 * @brief �J�����̃A�b�v�f�[�g
		 * @param playerPosition �v���C���[�̍��W
		*/
		void Update(const Vector3& playerPosition);

		/**
		 * @brief �ؒf���[�h�̃J�����̃A�b�v�f�[�g
		 * @param playerPosition �v���C���[�̍��W
		 * @param playerDirection �v���C���[�̌���
		 * @return 
		*/
		Vector3 UpdateCutMode(const Vector3& playerPosition,const Vector3& playerDirection);

		/**
		 * @brief �g�h���J�����̃A�b�v�f�[�g
		 * @param animParam 
		*/
		void UpdateFinishCamera(PlayerAnimationParam& animParam);

		/**
		 * @brief �J�����̍��W���擾
		 * @return �J�����̍��W
		*/
		const Vector3& GetCameraPosition()
		{
			return m_cameraPosition;
		}
		
		/**
		 * @brief �J�����̒����_���擾
		 * @return �����_�̍��W
		*/
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
		float m_lerpValue = 0.0f;						//�J�����̈ʒu�̐��`��ԗ�
		BackGround* m_backGround = nullptr;				//�w�i�N���X�̃|�C���^(�o�l�J�����p)
	};
}

