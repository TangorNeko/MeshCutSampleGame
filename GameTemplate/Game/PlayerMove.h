#pragma once
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerMove
	{
	public:

		void Init(Vector3& playerPosition);

		bool Move(Vector3& playerPosition,PlayerAnimationParam& animParam);

		void CalcToModelDirectionQRot();

		const Vector3& GetPlayerDirection()
		{
			return m_playerDirection;
		}

		void SetPlayerDirection(const Vector3& direction)
		{
			m_playerDirection = direction;
		}

		CharacterController& GetCharaCon()
		{
			return m_charaCon;
		}

		const Quaternion& GetPlayerDirectionRot()
		{
			return m_toMoveDirectionRot;
		}

		void KnockDown(const Vector3& moveAmount);

		void BackHandSpring(const Vector3& moveAmount);

		void NoticeMissileMoveEnd()
		{
			m_isMissileMove = false;
		}

		void NoticeFrontMoveEnd()
		{
			m_isFrontMove = false;
		}
	private:
		CharacterController m_charaCon;							//�v���C���[�̃L�����R��
		Vector3 m_moveAmount = Vector3::Zero;					//�v���C���[�̈ړ���
		Vector3 m_prevPosition = Vector3::Zero;					//1�t���[���O�̃v���C���[�̍��W
		Vector3 m_playerDirection = Vector3::Front;				//�v���C���[�̌���
		Quaternion m_toMoveDirectionRot = Quaternion::Identity;	//�v���C���[�̈ړ������ւ̃N�H�[�^�j�I��
		int m_aerialFrame = 0;									//�󒆂ɂ���t���[����

		//TODO:�ϐ��̕K�v�������؂����̂�������m_���������o�[�ϐ��Ƃ���
		bool isKnockDown = false;
		int knockDownFrame = 0;
		Vector3 knockDownAmount = Vector3::Zero;

		//TODO:���~�T�C���W�����v�p�ϐ�
		bool m_isMissileMove = false;
		Vector3 m_targetPos[5] = { Vector3::Zero,Vector3::Zero,Vector3::Zero ,Vector3::Zero,Vector3::Zero };
		int m_moveState = 0;
		int m_targetCount = 0;
		int m_jumpFrameCount = 0;
		int m_distanceCount = 0;
		bool m_isMovingUp = true;
		bool m_isMoveStartFrame = true;

		//TODO:�{�X�O�W�����v�p�ϐ�
		bool m_isFrontMove = false;
		bool m_isJumpStartFrame = true;
		Vector3 m_frontMoveAmount = Vector3::Zero;

		//TODO:��]�p�ϐ�
		bool isBackHandspring = false;
		int backHandspringFrame = 0;
		Vector3 backHandSpringAmount = Vector3::Zero;
	};
}

