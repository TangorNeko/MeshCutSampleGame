#include "stdafx.h"
#include "PlayerMove.h"
#include "StepObject.h"
#include "BossTank.h"

namespace
{
	const float PLAYER_HEIGHT = 120.0f;
	const float PLAYER_RADIUS = 30.0f;
	const float MOVE_SPEED = 5.0f;
	const float DASH_SPEED = 10.0f;
	const EnButton BUTTON_RUN = enButtonRB2;
	const float MISSILEJUMP_SPEED = 30.0f;
	const wchar_t* LANDING_SOUND_PATH = L"Assets/sound/LandingSE.wav";
}

namespace Game
{
	void PlayerMove::Init(Vector3& playerPosition)
	{
		//�L�����R����������
		m_charaCon.Init(PLAYER_RADIUS, PLAYER_HEIGHT,playerPosition);
	}

	bool PlayerMove::Move(Vector3& playerPosition, PlayerAnimationParam& animParam)
	{
		switch (m_playerMoveEvent)
		{
		case enNormal:
			return NormalMove(playerPosition, animParam);
			break;
		case enMissileMove:
			return MissileMove(playerPosition, animParam);
			break;
		case enFrontMove:
			return FrontMove(playerPosition, animParam);
			break;
		case enBackHandspring:
			return BackHandspringMove(playerPosition, animParam);
			break;
		case enKnockDown:
			return KnockDownMove(playerPosition, animParam);
			break;
		default :
			return false;
		}
	}

	void PlayerMove::KnockDown(const Vector3& moveAmount)
	{
		m_playerMoveEvent = enKnockDown;
		knockDownAmount = moveAmount;
	}

	void PlayerMove::BackHandSpring(const Vector3& moveAmount)
	{
		m_playerMoveEvent = enBackHandspring;
		backHandSpringAmount = moveAmount;
	}

	void PlayerMove::CalcToModelDirectionQRot()
	{
		//�ړ�������x,z�����]�p�x���擾
		float turnAngle = atan2(m_playerDirection.x, m_playerDirection.z);

		//��]�p�x���̃N�H�[�^�j�I�����쐬
		m_toMoveDirectionRot = Quaternion::Identity;
		m_toMoveDirectionRot.SetRotation(Vector3::AxisY, turnAngle);
	}


	//TODO:�N���X����
	bool PlayerMove::NormalMove(Vector3& playerPosition, PlayerAnimationParam& animParam)
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

		if (m_charaCon.IsOnGround() == true)
		{
			m_aerialFrame = 0;
		}
		Vector3 Down = Vector3::Down * m_aerialFrame;

		m_moveAmount += Down;
		//�L�����R���ɓn���B
		playerPosition = m_charaCon.Execute(m_moveAmount, 1.0f);

		m_aerialFrame++;

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

		return false;
	}

	bool PlayerMove::MissileMove(Vector3& playerPosition, PlayerAnimationParam& animParam)
	{
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			m_isMissileMove = true;
			//�{�X�̕��Ɍ�������
			SetPlayerDirection(Vector3::Back);
		}
		//NOTE:���̃~�T�C���W�����v����
		if (m_isMissileMove)
		{
			m_targetCount = 1;

			animParam.isJumping = true;

			//�ǐՂ���^�[�Q�b�g���擾�@�܂�3���݂��邱�Ƃ����z�肵�Ă��Ȃ�]
			//�ǐՊJ�n�̏u�Ԃ���QueryGOs���Ă��������A�W�����v�悪�����Ɠ��B���ɂ͑啪���W������Ă����̂�
			//����A�{�^���������Ă���Ԗ��t���[���������Ă���
			QueryGOs<StepObject>("stepObject", [this](StepObject* targetObject)->bool
				{
					m_targetPos[m_targetCount] = targetObject->GetPosition();
					m_targetCount++;
					return true;
				}
			);

			//���݂̍��W
			m_targetPos[0] = playerPosition;

			//��:��ԖC�g���̍��W
			m_targetPos[4] = { 300.0f,250.0f,-1800.0f };

			if (m_moveState == 4)
			{

				Vector3 LastPos = { 300.0f,250.0f,-1600.0f };

				Vector3 dis = LastPos - playerPosition;

				if (dis.LengthSq() < 300.0f * 300.0f)
				{
					animParam.isJumping = false;

					m_isMissileMove = false;
					return true;
				}
			}

			//���݂̃^�[�Q�b�g�ւ̃x�N�g�������߂�
			Vector3 distance = m_targetPos[m_moveState] - playerPosition;

			//�^�[�Q�b�g����50�ȓ��Ȃ瓞�B����
			if (distance.LengthSq() < 50.0f * 50.0f)
			{
				//���̃^�[�Q�b�g��
				m_moveState++;

				//���B�����̂Ŏ��̃^�[�Q�b�g�ֈړ�����ŏ��̃t���[��
				m_isMoveStartFrame = true;

				//�W�����v�t���[�������Z�b�g
				m_jumpFrameCount = 0;

				animParam.isJumping = false;

				SoundOneShotPlay(LANDING_SOUND_PATH);
			}

			//���̃^�[�Q�b�g�Ɉړ�����ŏ��̃t���[���Ȃ�
			if (m_isMoveStartFrame == true)
			{
				//���݂̃^�[�Q�b�g�ʒu���玟�̃^�[�Q�b�g�ʒu�ւ̋����𑪒�
				Vector3 distanceBetweenTargets = m_targetPos[m_moveState - 1] - m_targetPos[m_moveState];
				distanceBetweenTargets.y = 0.0f;
				//�������ړ����x�Ŋ����Ĉړ��ɂ�����t���[�����v�Z
				m_distanceCount = distanceBetweenTargets.Length() / MISSILEJUMP_SPEED;
				m_isMoveStartFrame = false;
				animParam.isJumping = false;
			}

			//�W�����v���牽�t���[���ڂ���̃J�E���g���C���N�������g
			m_jumpFrameCount++;

			//�ړ��ɂ�����t���[���̔����܂ł͏�ړ��A�߂���Ɖ��ړ�
			if (m_jumpFrameCount >= m_distanceCount / 2)
			{
				m_isMovingUp = false;
			}
			else
			{
				m_isMovingUp = true;
			}

			//�㉺�ړ��p�̃x�N�g��
			Vector3 jumpMoveVector = Vector3::Zero;

			if (m_jumpFrameCount <= m_distanceCount)
			{
				if (m_isMovingUp)
				{
					//��ړ����Ȃ��ړ��x�N�g�����i�[
					jumpMoveVector = Vector3::Up * 5.0f * m_jumpFrameCount;
				}
				else
				{
					//���ړ����Ȃ牺�ړ��x�N�g�����i�[
					jumpMoveVector = Vector3::Up * 5.0f * (m_distanceCount - m_jumpFrameCount);
				}
			}

			//���݂̃^�[�Q�b�g�ւ̃x�N�g���𐳋K�����đ傫��1�ɂ���
			distance.Normalize();

			//�ړ����x����Z
			distance *= MISSILEJUMP_SPEED;

			//�㉺�ړ����̃x�N�g�������Z�B
			//distance += jumpMoveVector;

			//�L�����R���Ɏ��s������B
			playerPosition = m_charaCon.Execute(distance, 1.0f);

			playerPosition += jumpMoveVector;
		}

		return false;
	}

	bool PlayerMove::FrontMove(Vector3& playerPosition, PlayerAnimationParam& animParam)
	{
		animParam.isJumping = true;

		if (m_isJumpStartFrame)
		{
			BossTank* bossTank = FindGO<BossTank>("bosstank");
			Vector3 targetPos = bossTank->GetFrontPosition();

			m_frontMoveAmount = targetPos - playerPosition;
			m_frontMoveAmount /= 25.0f;
			m_jumpFrameCount = 0;
			m_isJumpStartFrame = false;
		}

		if (m_jumpFrameCount < 25)
		{
			playerPosition += m_frontMoveAmount;
		}
		if (m_jumpFrameCount == 25)
		{
			animParam.isJumping = false;
			m_charaCon.SetPosition(playerPosition);

			m_isFrontMove = false;
			return true;
		}

		m_jumpFrameCount++;

		return false;
	}

	bool PlayerMove::BackHandspringMove(Vector3& playerPosition, PlayerAnimationParam& animParam)
	{
		animParam.isBackHandSpring = true;

		if (backHandspringFrame <= 40)
		{
			Vector3 gravity = { 0.0f,-10.0f,0.0f };
			playerPosition = m_charaCon.Execute(backHandSpringAmount, 1.0f);
			playerPosition = m_charaCon.Execute(gravity, 1.0f);

			Vector3 direction = backHandSpringAmount * -1.0f;
			direction.y = 0.0f;

			direction.Normalize();
			m_playerDirection = direction;
		}

		backHandspringFrame++;
		animParam.handspringTime = backHandspringFrame;

		if (backHandspringFrame == 100)
		{
			animParam.isBackHandSpring = false;
			backHandspringFrame = 0;

			m_playerMoveEvent = enNormal;
		}
		return false;
	}

	bool PlayerMove::KnockDownMove(Vector3& playerPosition, PlayerAnimationParam& animParam)
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
			animParam.isKnockDown = false;
			knockDownFrame = 0;

			m_playerMoveEvent = enNormal;
		}
		return false;
	}
}