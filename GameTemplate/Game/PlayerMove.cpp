#include "stdafx.h"
#include "PlayerMove.h"
#include "StepObject.h"
#include "BossTank.h"

namespace
{
	const float PLAYER_HEIGHT = 120.0f;										//�v���C���[�̓����蔻��̃J�v�Z���̍���
	const float PLAYER_RADIUS = 30.0f;										//�v���C���[�̓����蔻��̃J�v�Z���̔��a
	const float WALK_SPEED = 5.0f;											//�v���C���[�̕������x
	const float DASH_SPEED = 10.0f;											//�v���C���[�̑��葬�x
	const EnButton BUTTON_RUN = enButtonRB2;								//�_�b�V���{�^��
	const float MISSILEJUMP_SPEED = 30.0f;									//�~�T�C���ړ��̈ړ����x
	const wchar_t* LANDING_SOUND_PATH = L"Assets/sound/LandingSE.wav";		//���n���̌��ʉ��p�X
	const Vector3 TANK_RIGHT_POSITION = { 300.0f,250.0f,-1800.0f };			//��Ԃ̉E�̃X�y�[�X�̍��W
	const Vector3 MISSILEMOVE_END_POSITION = { 300.0f,250.0f,-1600.0f };	//�~�T�C���ړ��̏I�����W
	const float MISSILEMOVE_END_DISTANCE = 300.0f;							//�~�T�C���ړ��̏I������
	const float MISSILEMOVE_REACH_DISTANCE = 50.0f;							//�~�T�C���ړ��̓��B����
	const float MISSILEMOVE_VERTICAL_SPEED = 5.0f;							//�~�T�C���ړ��̐��������̈ړ����x
	const float FRONTMOVE_DIVIDENUM = 25.0f;								//�{�X���ʂւ̈ړ��̏��Z�l(�ړ��������t���[�����Ŋ��邽��)
	const float FRONTMOVE_END = 25;											//�{�X���ʂւ̈ړ��̏I���t���[��
	const Vector3 BACKHANDSPRING_GRAVITY = { 0.0f,-10.0f,0.0f };			//��]�ړ��̏d��
	const float BACKHANDSPRING_MOVE = 40;									//��]�ړ��̈ړ��t���[��
	const float BACKHANDSPRING_END = 100;									//��]�ړ��̏I���t���[��
	const float KNOCKDOWN_MOVE = 40;										//������΂��̈ړ��t���[��
	const float KNOCKDOWN_END = 150;										//������΂��̏I���t���[��
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
		//�v���C���[�̈ړ���Ԃɂ���ĕ���
		switch (m_playerMoveEvent)
		{
		case enNormal://�ʏ�ړ�
			return NormalMove(playerPosition, animParam);
			break;
		case enMissileMove://�~�T�C���ړ�
			return MissileMove(playerPosition, animParam);
			break;
		case enFrontMove://�{�X�̐��ʂւ̈ړ�
			return FrontMove(playerPosition, animParam);
			break;
		case enBackHandspring://��]
			return BackHandspringMove(playerPosition, animParam);
			break;
		case enKnockDown://������΂�
			return KnockDownMove(playerPosition, animParam);
			break;
		case enDead://���S
			return DeadMove();
		default :
			return false;
		}
	}

	void PlayerMove::KnockDown(const Vector3& moveAmount)
	{
		//�ړ���Ԃ𐁂���΂���Ԃ�
		m_playerMoveEvent = enKnockDown;

		//�ړ��ʂ��Z�b�g
		m_knockDownAmount = moveAmount;
	}

	void PlayerMove::BackHandSpring(const Vector3& moveAmount)
	{
		//�ړ���Ԃ���]��Ԃ�
		m_playerMoveEvent = enBackHandspring;
		
		//�ړ��ʂ��Z�b�g
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
			m_moveAmount *= WALK_SPEED;
		}

		//�n�ʂɂ�����󒆂ɂ����t���[�������Z�b�g
		if (m_charaCon.IsOnGround() == true)
		{
			m_aerialFrame = 0;
		}

		//�v���C���[�ɂ�����d��
		Vector3 Down = Vector3::Down * m_aerialFrame;

		m_moveAmount += Down;
		//�L�����R���ɓn���B
		playerPosition = m_charaCon.Execute(m_moveAmount, 1.0f);

		m_aerialFrame++;



		//�A�j���[�V�����֘A
		//���W���ω����Ă����
		if(m_moveAmount.LengthSq() > FLT_EPSILON)
		{
			//����{�^���������Ă���΃_�b�V��
			if (g_pad[0]->IsPress(BUTTON_RUN))
			{
				animParam.playerState = PlayerAnimationParam::enRunning;
			}
			else
			{
				//�����Ă��Ȃ���Ε���
				animParam.playerState = PlayerAnimationParam::enWalk;
			}
		}
		else //�ω����Ă��Ȃ���Αҋ@
		{
			animParam.playerState = PlayerAnimationParam::enIdle;
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

		//NOTE:�~�T�C���W�����v����
		if (m_isMissileMove)
		{
			m_targetCount = 1;

			animParam.playerState = PlayerAnimationParam::enJumping;

			//�ǐՂ���^�[�Q�b�g���擾
			//�ǐՊJ�n�̏u�Ԃ���QueryGOs���Ă��������A�W�����v�悪�����Ɠ��B���ɂ͑啪���W������Ă����̂�
			//���t���[���������Ă���
			QueryGOs<StepObject>("stepObject", [this](StepObject* targetObject)->bool
				{
					m_targetPos[m_targetCount] = targetObject->GetPosition();
					m_targetCount++;
					return true;
				}
			);

			//���݂̍��W
			m_targetPos[0] = playerPosition;

			//��ԖC�g���̍��W
			m_targetPos[4] = TANK_RIGHT_POSITION;

			//�Ō�̃W�����v�Ȃ�
			if (m_moveState == 4)
			{
				//�I�����W���擾
				Vector3 LastPos = MISSILEMOVE_END_POSITION;

				//�I�����W�ւ̋������v�Z
				Vector3 dis = LastPos - playerPosition;

				//�I�����W�̋߂��ɓ��B������
				if (dis.LengthSq() < MISSILEMOVE_END_DISTANCE * MISSILEMOVE_END_DISTANCE)
				{
					//�ҋ@�A�j���[�V�����ɕύX
					animParam.playerState = PlayerAnimationParam::enIdle;

					//�~�T�C���ړ��̏I��
					m_isMissileMove = false;
					return true;
				}
			}

			//���݂̃^�[�Q�b�g�ւ̃x�N�g�������߂�
			Vector3 distance = m_targetPos[m_moveState] - playerPosition;

			//�^�[�Q�b�g����߂���Γ��B����
			if (distance.LengthSq() < MISSILEMOVE_REACH_DISTANCE * MISSILEMOVE_REACH_DISTANCE)
			{
				//���̃^�[�Q�b�g��
				m_moveState++;

				//���B�����̂Ŏ��̃^�[�Q�b�g�ֈړ�����ŏ��̃t���[��
				m_isMoveStartFrame = true;

				//�W�����v�t���[�������Z�b�g
				m_jumpFrameCount = 0;

				//�ҋ@�A�j���[�V�����ɂ���(��U�ҋ@�A�j���[�V���������ގ��ōēx�W�����v�A�j���[�V�������Đ������)
				animParam.playerState = PlayerAnimationParam::enIdle;

				//���n�T�E���h���Đ�
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

				animParam.playerState = PlayerAnimationParam::enIdle;
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
					jumpMoveVector = Vector3::Up * MISSILEMOVE_VERTICAL_SPEED * m_jumpFrameCount;
				}
				else
				{
					//���ړ����Ȃ牺�ړ��x�N�g�����i�[
					jumpMoveVector = Vector3::Up * MISSILEMOVE_VERTICAL_SPEED * (m_distanceCount - m_jumpFrameCount);
				}
			}

			//���݂̃^�[�Q�b�g�ւ̃x�N�g���𐳋K�����đ傫��1�ɂ���
			distance.Normalize();

			//�ړ����x����Z
			distance *= MISSILEJUMP_SPEED;

			//�L�����R���Ɏ��s������B
			playerPosition = m_charaCon.Execute(distance, 1.0f);

			playerPosition += jumpMoveVector;
		}

		return false;
	}

	bool PlayerMove::FrontMove(Vector3& playerPosition, PlayerAnimationParam& animParam)
	{
		//�W�����v�A�j���[�V�����ɂ���
		animParam.playerState = PlayerAnimationParam::enJumping;

		//�W�����v�J�n�t���[���Ȃ�
		if (m_isJumpStartFrame)
		{
			//�{�X�̐��ʂ̍��W���擾
			BossTank* bossTank = FindGO<BossTank>("bosstank");
			Vector3 targetPos = bossTank->GetFrontPosition();

			//�{�X���ʂɈړ�����ۂ�1�t���[���̈ړ��ʂ��v�Z����
			m_frontMoveAmount = targetPos - playerPosition;
			m_frontMoveAmount /= FRONTMOVE_DIVIDENUM;
			m_jumpFrameCount = 0;
			m_isJumpStartFrame = false;
		}

		//�ŏ��Ɍv�Z�����ړ��ʕ���ڲ԰���ړ�������
		if (m_jumpFrameCount < FRONTMOVE_END)
		{
			playerPosition += m_frontMoveAmount;
		}

		//�ŏI�t���[���Ȃ�
		if (m_jumpFrameCount == FRONTMOVE_END)
		{
			//�ҋ@�A�j���[�V�����ɕύX
			animParam.playerState = PlayerAnimationParam::enIdle;

			//�L�����R���̍��W��ݒ�
			m_charaCon.SetPosition(playerPosition);

			//���ʈړ��̏I��
			m_isFrontMove = false;
			return true;
		}

		//�ړ��t���[�������Z
		m_jumpFrameCount++;

		return false;
	}

	bool PlayerMove::BackHandspringMove(Vector3& playerPosition, PlayerAnimationParam& animParam)
	{
		//��]�A�j���[�V�����ɂ���
		animParam.playerState = PlayerAnimationParam::enBackHandSpring;

		//��]�ړ����Ȃ�
		if (backHandspringFrame <= BACKHANDSPRING_MOVE)
		{
			//�ݒ肵���ړ��ʂƏd�͕��L�����R�������s
			Vector3 gravity = BACKHANDSPRING_GRAVITY;
			playerPosition = m_charaCon.Execute(backHandSpringAmount, 1.0f);
			playerPosition = m_charaCon.Execute(gravity, 1.0f);

			//�v���C���[�̌������v�Z����
			Vector3 direction = backHandSpringAmount * -1.0f;
			direction.y = 0.0f;

			direction.Normalize();
			m_playerDirection = direction;
		}

		//�ړ��t���[�������Z
		backHandspringFrame++;

		//��]�̏I���t���[���Ȃ�
		if (backHandspringFrame == BACKHANDSPRING_END)
		{
			//�ҋ@�A�j���[�V�����ɖ߂�
			animParam.playerState = PlayerAnimationParam::enIdle;

			//�o�߃t���[�������Z�b�g
			backHandspringFrame = 0;

			//�ړ���Ԃ�ʏ�ɖ߂�
			m_playerMoveEvent = enNormal;
		}
		return false;
	}

	bool PlayerMove::KnockDownMove(Vector3& playerPosition, PlayerAnimationParam& animParam)
	{
		//������΂��A�j���[�V�����ɂ���
		animParam.playerState = PlayerAnimationParam::enKnockDown;

		//������΂��ړ����Ȃ�
		if (m_knockDownFrame <= KNOCKDOWN_MOVE)
		{
			//�L�����R���Ɉړ��ʂ�n��
			playerPosition = m_charaCon.Execute(m_knockDownAmount, 1.0f);
		}

		//�ړ��t���[�������Z
		m_knockDownFrame++;

		//������΂��̏I���t���[���Ȃ�
		if (m_knockDownFrame == KNOCKDOWN_END)
		{
			//�ҋ@�A�j���[�V�����ɖ߂�
			animParam.playerState = PlayerAnimationParam::enIdle;
			
			//�o�߃t���[�������Z�b�g
			m_knockDownFrame = 0;

			//�ړ���Ԃ�ʏ��Ԃɖ߂�
			m_playerMoveEvent = enNormal;
		}
		return false;
	}
}