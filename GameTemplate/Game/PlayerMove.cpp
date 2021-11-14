#include "stdafx.h"
#include "PlayerMove.h"

namespace
{
	const float PLAYER_HEIGHT = 10.0f;
	const float PLAYER_RADIUS = 5.0f;
	const float MOVE_SPEED = 5.0f;
}

namespace Game
{
	void PlayerMove::Init(Vector3& playerPosition)
	{
		//�L�����R����������
		m_charaCon.Init(PLAYER_RADIUS, PLAYER_HEIGHT,playerPosition);
	}

	void PlayerMove::Move(Vector3& playerPosition, PlayerAnimationParam& animParam)
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

		m_moveAmount *= MOVE_SPEED;

		//�L�����R���ɓn���B
		playerPosition = m_charaCon.Execute(m_moveAmount, 1.0f);



		//�A�j���[�V�����֘A�@�ォ�番�����悤
		
		//���W���ω����Ă���Ε����t���I��
		if (m_prevPosition.LengthSq() != playerPosition.LengthSq())
		{
			animParam.isWalking = true;
		}
		else //�ω����Ă��Ȃ���Ε����t���O�I�t
		{
			animParam.isWalking = false;
		}

		//���t���[���̍��W���i�[
		m_prevPosition = playerPosition;
	}

	Quaternion PlayerMove::CalcToModelDirectionQRot()
	{
		//�ړ��ʂ��ŏ��l�ȏ�(�ړ��ʂɕω���������)�̎��̂݌v�Z
		if (m_moveAmount.LengthSq() < FLT_MIN)
		{
			return m_toMoveDirectionRot;
		}

		//�ړ�������x,z�����]�p�x���擾
		float turnAngle = atan2(m_moveAmount.x, m_moveAmount.z);

		//��]�p�x���̃N�H�[�^�j�I�����쐬
		m_toMoveDirectionRot = Quaternion::Identity;
		m_toMoveDirectionRot.SetRotation(Vector3::AxisY, turnAngle);

		return m_toMoveDirectionRot;
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