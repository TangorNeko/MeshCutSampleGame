#include "stdafx.h"
#include "PlayerMove.h"

namespace
{
	const float PLAYER_HEIGHT = 10.0f;
	const float PLAYER_RADIUS = 5.0f;
}

namespace Game
{
	void PlayerMove::Init(Vector3& playerPosition)
	{
		//�L�����R����������
		m_charaCon.Init(PLAYER_RADIUS, PLAYER_HEIGHT,playerPosition);
	}

	void PlayerMove::Move(Vector3& playerPosition)
	{
		//�e���̓��͂��擾
		float rightMoveAmount = g_pad[0]->GetLStickXF();
		float forwardMoveAmount = g_pad[0]->GetLStickYF();

		//�J�����̑O�������擾
		Vector3 cameraFront = g_camera3D->GetForward();
		cameraFront.y = 0.0f;
		cameraFront.Normalize();

		//�J�����̉E�������擾
		Vector3 cameraRight = g_camera3D->GetRight();
		cameraRight.y = 0.0f;
		cameraRight.Normalize();


		//�ړ������̓J�����̑O���� * Y�����ւ̓��͗� + �J�����̉E���� * X�����ւ̓��͗�
		m_moveAmount = cameraFront * forwardMoveAmount + cameraRight * rightMoveAmount;

		//�ړ������𐳋K��
		m_moveAmount.Normalize();

		//�L�����R���ɓn���B
		playerPosition = m_charaCon.Execute(m_moveAmount, 1.0f);
	}

	Quaternion PlayerMove::CalcToModelDirectionQRot()
	{
		//�ړ�������x,z�����]�p�x���擾
		float turnAngle = atan2(m_moveAmount.x, m_moveAmount.z);

		//��]�p�x���̃N�H�[�^�j�I�����쐬
		Quaternion toMoveDirectionRot;
		toMoveDirectionRot.SetRotation(Vector3::AxisY, turnAngle);

		return toMoveDirectionRot;
	}

	void PlayerMove::TurnModelToMoveDirection(SkinModelRender* modelRender)
	{
		//�ړ��ʂ�����Ȃ����Ȃ��ꍇ������ύX���Ȃ�
		if (m_moveAmount.LengthSq() > FLT_MIN && modelRender != nullptr)
		{
			//���f���ɉ�]�𔽉f������B
			modelRender->SetRotation(CalcToModelDirectionQRot());
		}
	}
}