#include "stdafx.h"
#include "PlayerMove.h"

namespace
{
	const float PLAYER_HEIGHT = 120.0f;
	const float PLAYER_RADIUS = 30.0f;
	const float MOVE_SPEED = 5.0f;
	const float DASH_SPEED = 10.0f;
	const EnButton BUTTON_RUN = enButtonRB2;
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
		//TODO:���ꓖ���菈���AKnockBackMove�Ƃ��ɕ�����������������...
		if (isKnockDown)
		{
			animParam.isKnockDown = true;

			if (knockDownFrame <= 40)
			{
				playerPosition = m_charaCon.Execute(knockDownAmount, 1.0f);
			}

			knockDownFrame++;
			animParam.downTime = knockDownFrame;

			if (knockDownFrame == 150)
			{
				isKnockDown = false;
				animParam.isKnockDown = false;
				knockDownFrame = 0;
			}
			return;
		}

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

		//�ړ��ʂ��ŏ��l�ȏ�(�ړ��ʂɕω���������)�̎��̂݌������i�[
		if (m_moveAmount.LengthSq() > FLT_EPSILON)
		{
			m_playerDirection = m_moveAmount;
		}

		//����{�^���������Ă���Α���
		if (g_pad[0]->IsPress(BUTTON_RUN))
		{
			m_moveAmount *= DASH_SPEED;
		}
		else
		{
			m_moveAmount *= MOVE_SPEED;
		}

		//�L�����R���ɓn���B
		playerPosition = m_charaCon.Execute(m_moveAmount, 1.0f);



		//�A�j���[�V�����֘A�@�ォ�番�����悤
		
		//���W���ω����Ă����
		if (m_prevPosition.LengthSq() != playerPosition.LengthSq())
		{
			//����{�^���������Ă���΃_�b�V��
			if (g_pad[0]->IsPress(BUTTON_RUN))
			{
				animParam.isRunning = true;
				animParam.isWalking = false;
			}
			else
			{
				//�����Ă��Ȃ���Ε���
				animParam.isWalking = true;
				animParam.isRunning = false;
			}
		}
		else //�ω����Ă��Ȃ���Ε����A����t���O�I�t
		{
			animParam.isWalking = false;
			animParam.isRunning = false;
		}

		//���t���[���̍��W���i�[
		m_prevPosition = playerPosition;
	}

	void PlayerMove::KnockDown(const Vector3& moveAmount)
	{
		isKnockDown = true;
		knockDownAmount = moveAmount;
	}

	void PlayerMove::CalcToModelDirectionQRot()
	{
		//�ړ�������x,z�����]�p�x���擾
		float turnAngle = atan2(m_playerDirection.x, m_playerDirection.z);

		//��]�p�x���̃N�H�[�^�j�I�����쐬
		m_toMoveDirectionRot = Quaternion::Identity;
		m_toMoveDirectionRot.SetRotation(Vector3::AxisY, turnAngle);
	}
}