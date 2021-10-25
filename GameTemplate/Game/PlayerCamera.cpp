#include "stdafx.h"
#include "PlayerCamera.h"

namespace
{
	const Vector3 PLAYER_TO_TARGET = { 0.0f,150.0f,0.0f };
	const float TOCAMERA_LENGTH = 500.0f;
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

		float degY = g_pad[0]->GetRStickXF() * 0.5f;
		float degX = g_pad[0]->GetRStickYF() * -0.5f;

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
}