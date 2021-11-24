#include "stdafx.h"
#include "PlayerCamera.h"

namespace
{
	const Vector3 PLAYER_TO_TARGET = { 0.0f,150.0f,0.0f };
	const float TOCAMERA_LENGTH = 500.0f;
	const Vector3 CUTMODE_PLAYER_TO_TARGET = { 50.0f,160.0f,0.0f };
	const float CUTMODE_TOCAMERA_LENGTH = 80.0f;
	const float CAMERA_SPEED = 2.0f;
}

namespace Game
{
	void PlayerCamera::Update(const Vector3& playerPosition)
	{
		//�J�����̃^�[�Q�b�g�̓v���C���[�̈ʒu����v�Z����
		m_cameraTarget = playerPosition;
		m_cameraTarget += PLAYER_TO_TARGET;

		g_camera3D->SetTarget(m_cameraTarget);

		//�������A�c�����̓��͂��擾
		Quaternion qRotY = Quaternion::Identity;
		Quaternion qRotX = Quaternion::Identity;

		float degY = g_pad[0]->GetRStickXF() * CAMERA_SPEED;
		float degX = g_pad[0]->GetRStickYF() * -CAMERA_SPEED;

		//���͕���
		qRotY.SetRotationDegY(degY);

		//�J�����̏㉺�ړ��̎��̓J�����̉E�Ƃ���
		qRotX.SetRotationDeg(g_camera3D->GetRight(),degX);

		//�J�����^�[�Q�b�g����J�����܂ł̌����ɉ�]��K�p
		qRotY.Apply(m_toCameraDirection);
		qRotX.Apply(m_toCameraDirection);

		//�J�����^�[�Q�b�g�ʒu�ɃJ�����܂ł̌���*�J�����܂ł̒����������ăJ�����ʒu���擾
		Vector3 cameraPosition = m_cameraTarget + m_toCameraDirection * TOCAMERA_LENGTH;

		//�J�����ʒu���Z�b�g
		g_camera3D->SetPosition(cameraPosition);
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
		m_cameraTarget = playerPosition;
		m_cameraTarget += toTarget;

		g_camera3D->SetTarget(m_cameraTarget);

		//�������A�c�����̓��͂��擾
		Quaternion qRotY = Quaternion::Identity;
		Quaternion qRotX = Quaternion::Identity;

		//�ؒf���[�h���͍��X�e�B�b�N�ŃJ�����ړ�������
		float degY = g_pad[0]->GetLStickXF() * CAMERA_SPEED;
		float degX = g_pad[0]->GetLStickYF() * -CAMERA_SPEED;

		//���͕���
		qRotY.SetRotationDegY(degY);

		//�J�����̏㉺�ړ��̎��̓J�����̉E�Ƃ���
		qRotX.SetRotationDeg(g_camera3D->GetRight(), degX);

		//�J�����^�[�Q�b�g����J�����܂ł̌����ɉ�]��K�p
		qRotY.Apply(m_toCameraDirection);
		qRotX.Apply(m_toCameraDirection);

		//�J�����^�[�Q�b�g�ʒu�ɃJ�����܂ł̌���*�J�����܂ł̒����������ăJ�����ʒu���擾
		Vector3 cameraPosition = m_cameraTarget + m_toCameraDirection * CUTMODE_TOCAMERA_LENGTH;

		//�J�����ʒu���Z�b�g
		g_camera3D->SetPosition(cameraPosition);

		//�J�����ړ���̐V�����v���C���[�̌�����Ԃ�
		Vector3 newDirection = playerDirection;
		qRotY.Apply(newDirection);
		return newDirection;
	}
}