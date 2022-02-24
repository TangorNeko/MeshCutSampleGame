#include "stdafx.h"
#include "PlayerCamera.h"
#include "BackGround.h"

namespace
{
	const Vector3 PLAYER_TO_TARGET = { 0.0f,150.0f,0.0f };			//�v���C���[���璍���_�ւ̃x�N�g��
	const float TOCAMERA_LENGTH = 500.0f;							//�J�����ւ̃x�N�g���̒���
	const Vector3 CUTMODE_PLAYER_TO_TARGET = { 45.0f,160.0f,0.0f };	//�ؒf���[�h�̃v���C���[���璍���_�ւ̃x�N�g��
	const float CUTMODE_TOCAMERA_LENGTH = 80.0f;					//�ؒf���[�h�̃J�����ւ̃x�N�g���̒���
	const float CAMERA_SPEED = 2.0f;								//�J�����̈ړ����x
	const float CAMERA_MAX_DEG_X = 45.0f;							//�J������X������̍ő�p�x
	const float SPRINGCAMERA_SPACE = 10.0f;							//�΂˃J�����̃X�y�[�X�̒���
	const float CAMERA_LERP_INCREASE_VALUE = 0.01f;					//�J�n���̃J�����̐��`��Ԃ̐i�s�x
	const float CAMERA_LERPVALUE_MAX = 1.0f;						//�J�n���̃J�����̐��`��Ԃ̍ő�l
	const Vector3 FINISH_CAMERA_POSITION = { 0.0f,250.0f,-700.0f };	//�g�h���J�����̍��W
	const Vector3 FINISH_CAMERA_TARGET = { 0.0f,0.0f,-1300.0f };	//�g�h���J�����̒����_
}

namespace Game
{
	PlayerCamera::PlayerCamera()
	{
		//�΂˃J�����p�ɃQ�[���̔w�i�̃C���X�^���X���擾
		m_backGround = FindGO<BackGround>("background");
	}

	void PlayerCamera::Update(const Vector3& playerPosition)
	{
		//�⊮���𑝉�������
		m_lerpValue += CAMERA_LERP_INCREASE_VALUE;
		m_lerpValue = min(m_lerpValue, CAMERA_LERPVALUE_MAX);

		//�J�����̃^�[�Q�b�g�̓v���C���[�̈ʒu����v�Z����
		m_cameraTarget = playerPosition;
		m_cameraTarget += PLAYER_TO_TARGET;

		Vector3 lerptarget = Vector3::Zero;
		lerptarget.Lerp(m_lerpValue, m_cutCameraTarget, m_cameraTarget);

		g_camera3D->SetTarget(lerptarget);

		//�������A�c�����̓��͂��擾
		Quaternion qRotY = Quaternion::Identity;
		Quaternion qRotX = Quaternion::Identity;

		float degY = g_pad[0]->GetRStickXF() * CAMERA_SPEED;
		float degX = g_pad[0]->GetRStickYF() * -CAMERA_SPEED;

		//X���J�����̊p�x����
		m_totalDegX += degX;
		if (m_totalDegX <= -CAMERA_MAX_DEG_X || m_totalDegX >= CAMERA_MAX_DEG_X)
		{
			m_totalDegX -= degX;
			degX = 0;
		}

		//���͕���
		qRotY.SetRotationDegY(degY);

		//�J�����̏㉺�ړ��̎��̓J�����̉E�Ƃ���
		qRotX.SetRotationDeg(g_camera3D->GetRight(),degX);

		//�J�����^�[�Q�b�g����J�����܂ł̌����ɉ�]��K�p
		qRotY.Apply(m_toCameraDirection);
		qRotX.Apply(m_toCameraDirection);

		//�J�����^�[�Q�b�g�ʒu�ɃJ�����܂ł̌���*�J�����܂ł̒����������ăJ�����ʒu���擾
		m_cameraPosition = m_cameraTarget + m_toCameraDirection * TOCAMERA_LENGTH;

		Vector3 pos = Vector3::Zero;

		Vector3 crossPoint;
		//�J�����ʒu�ւ̐������X�e�[�W�Ɠ������Ă��邩�`�F�b�N
		bool isHit = m_backGround->isLineHitModel(m_cameraTarget, m_cameraPosition, crossPoint);

		//�X�e�[�W�ɓ���������
		if (isHit == true)
		{
			Vector3 toCameraDirection = m_cameraPosition - m_cameraTarget;
			toCameraDirection.Normalize();
			//���̂܂܂��ƕǂɖ��܂�̂ŏ����X�y�[�X��������
			pos = crossPoint - toCameraDirection * SPRINGCAMERA_SPACE;
		}
		else
		{
			//�J�����ʒu���Z�b�g
			pos = m_cameraPosition;
		}

		//���W����`��Ԃ���
		Vector3 lerppos = Vector3::Zero;
		lerppos.Lerp(m_lerpValue, m_cutCameraPosition, pos);
		g_camera3D->SetPosition(lerppos);
	}

	Vector3 PlayerCamera::UpdateCutMode(const Vector3& playerPosition, const Vector3& playerDirection)
	{
		//�v���C���[�̌��݂̌����܂ł̉�]���Z�b�g
		Quaternion toDirection = Quaternion::Identity;
		toDirection.SetRotation(Vector3::Front, playerDirection);

		//�v���C���[�̌����܂ł̉�]���^�[�Q�b�g�ւ̃x�N�g���ɓK�p
		Vector3 toTarget = CUTMODE_PLAYER_TO_TARGET;
		toDirection.Apply(toTarget);

		//�J�����̃^�[�Q�b�g�̓v���C���[�̈ʒu����v�Z����
		m_cutCameraTarget = playerPosition;
		m_cutCameraTarget += toTarget;

		g_camera3D->SetTarget(m_cutCameraTarget);

		//�������A�c�����̓��͂��擾
		Quaternion qRotY = Quaternion::Identity;
		Quaternion qRotX = Quaternion::Identity;

		//�ؒf���[�h���͍��X�e�B�b�N�ŃJ�����ړ�������
		float inputDegY = g_pad[0]->GetLStickXF() * CAMERA_SPEED;
		float inputDegX = g_pad[0]->GetLStickYF() * -CAMERA_SPEED;

		//X���J�����̊p�x����
		m_totalDegX += inputDegX;
		if (m_totalDegX <= -CAMERA_MAX_DEG_X || m_totalDegX >= CAMERA_MAX_DEG_X)
		{
			m_totalDegX -= inputDegX;
			inputDegX = 0;
		}

		//���͕���
		qRotY.SetRotationDegY(inputDegY);

		//�J�����̏㉺�ړ��̎��̓J�����̉E�Ƃ���
		qRotX.SetRotationDeg(g_camera3D->GetRight(), inputDegX);

		//�J�����^�[�Q�b�g����J�����܂ł̌����ɉ�]��K�p
		qRotY.Apply(m_toCameraDirection);
		qRotX.Apply(m_toCameraDirection);

		//�J�����^�[�Q�b�g�ʒu�ɃJ�����܂ł̌���*�J�����܂ł̒����������ăJ�����ʒu���擾
		m_cutCameraPosition = m_cutCameraTarget + m_toCameraDirection * CUTMODE_TOCAMERA_LENGTH;

		//�J�����ʒu���Z�b�g
		g_camera3D->SetPosition(m_cutCameraPosition);

		//�J�����ړ���̐V�����v���C���[�̌�����Ԃ�
		//TODO:���݂̏����͐ؒf���[�h�ɓ������ۃv���C���[�̌������J�����ɒǏ]����悤�ɂȂ��Ă��邪�A
		//MGR�{�Ƃł̓v���C���[�̌����ɃJ�������Ǐ]���鏈���ɂȂ��Ă����B�ύX���Ă����������B
		Vector3 front = g_camera3D->GetForward();
		front.y = 0.0f;
		front.Normalize();
		return front;
	}

	void PlayerCamera::UpdateFinishCamera(PlayerAnimationParam& animParam)
	{
		//�g�h���J�������J�n���ꂽ��[������
		animParam.playerState = PlayerAnimationParam::enUnequip;

		//�g�h���J�����p�̍��W�ƒ����_��ݒ�
		g_camera3D->SetPosition(FINISH_CAMERA_POSITION);
		g_camera3D->SetTarget(FINISH_CAMERA_TARGET);
	}
}