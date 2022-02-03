#pragma once
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerMove
	{
	public:

		/**
		 * @brief ������
		 * @param playerPosition �v���C���[�̍��W
		*/
		void Init(Vector3& playerPosition);

		/**
		 * @brief �ړ�����
		 * @param playerPosition �v���C���[�̍��W
		 * @param animParam �v���C���[�̃A�j���[�V�����p�����[�^�[
		 * @return 
		*/
		bool Move(Vector3& playerPosition,PlayerAnimationParam& animParam);

		/**
		 * @brief ���f���̌����̃N�H�[�^�j�I�����v�Z����
		*/
		void CalcToModelDirectionQRot();

		/**
		 * @brief �v���C���[�̌������擾
		 * @return �v���C���[�̌���
		*/
		const Vector3& GetPlayerDirection()
		{
			return m_playerDirection;
		}

		/**
		 * @brief �v���C���[�̌������Z�b�g
		 * @param direction �v���C���[�̌���
		*/
		void SetPlayerDirection(const Vector3& direction)
		{
			m_playerDirection = direction;
		}

		/**
		 * @brief �v���C���[�̃L�����R�����擾
		 * @return �v���C���[�̃L�����R��
		*/
		CharacterController& GetCharaCon()
		{
			return m_charaCon;
		}

		/**
		 * @brief �v���C���[�̌����ւ̃N�H�[�^�j�I�����擾
		 * @return 
		*/
		const Quaternion& GetPlayerDirectionRot()
		{
			return m_toMoveDirectionRot;
		}

		/**
		 * @brief �v���C���[�𐁂���΂�
		 * @param moveAmount ������΂���
		*/
		void KnockDown(const Vector3& moveAmount);

		/**
		 * @brief �v���C���[�Ɍ�]������
		 * @param moveAmount ��]�̈ړ���
		*/
		void BackHandSpring(const Vector3& moveAmount);

		/**
		 * @brief �~�T�C�����шڂ鏈���̃X�^�[�g��ʒm����
		*/
		void NoticeMissileMoveStart()
		{
			m_playerMoveEvent = enMissileMove;
		}

		/**
		 * @brief �{�X�̐��ʂɈړ����鏈���̃X�^�[�g��ʒm����
		*/
		void NoticeFrontMoveStart()
		{
			m_playerMoveEvent = enFrontMove;

			//�{�X�̕��Ɍ�������
			SetPlayerDirection(Vector3::Back);
		}

		/**
		 * @brief �~�T�C�����шڂ鏈���̏I����ʒm����
		*/
		void NoticeMissileMoveEnd()
		{
			m_isMissileMove = false;

			m_playerMoveEvent = enNormal;
		}

		/**
		 * @brief �{�X�̐��ʂɈړ����鏈���̏I����ʒm����
		*/
		void NoticeFrontMoveEnd()
		{
			m_isFrontMove = false;

			m_playerMoveEvent = enNormal;
		}

		//�v���C���[�̈ړ���s�ɂ���
		void DisableMove()
		{
			m_playerMoveEvent = enDead;
		}

		/**
		 * @brief �ʏ�̈ړ�����
		 * @param playerPosition �v���C���[�̍��W
		 * @param animParam �v���C���[�̃A�j���[�V�����p�����[�^�[
		 * @return 
		*/
		bool NormalMove(Vector3& playerPosition, PlayerAnimationParam& animParam);

		/**
		 * @brief �~�T�C�����шڂ�ړ�����
		 * @param playerPosition �v���C���[�̍��W
		 * @param animParam �v���C���[�̃A�j���[�V�����p�����[�^�[
		 * @return
		*/
		bool MissileMove(Vector3& playerPosition, PlayerAnimationParam& animParam);

		/**
		 * @brief �{�X�̐��ʂɈړ����鏈��
		 * @param playerPosition �v���C���[�̍��W
		 * @param animParam �v���C���[�̃A�j���[�V�����p�����[�^�[
		 * @return
		*/
		bool FrontMove(Vector3& playerPosition, PlayerAnimationParam& animParam);

		/**
		 * @brief ��]�̈ړ�����
		 * @param playerPosition �v���C���[�̍��W
		 * @param animParam �v���C���[�̃A�j���[�V�����p�����[�^�[
		 * @return
		*/
		bool BackHandspringMove(Vector3& playerPosition, PlayerAnimationParam& animParam);

		/**
		 * @brief ������΂��̈ړ�����
		 * @param playerPosition �v���C���[�̍��W
		 * @param animParam �v���C���[�̃A�j���[�V�����p�����[�^�[
		 * @return
		*/
		bool KnockDownMove(Vector3& playerPosition, PlayerAnimationParam& animParam);

		/**
		 * @brief ���S���̈ړ�����(�������Ȃ�)
		 * @return 
		*/
		bool DeadMove()
		{
			return false;
		}

		/**
		 * @brief �~�T�C�����шڂ鏈���̃R�}���h�̐�����ʒm����
		*/
		void MissileMoveSuccess()
		{
			m_isMissileMove = true;
		}

	private:
		CharacterController m_charaCon;							//�v���C���[�̃L�����R��
		Vector3 m_moveAmount = Vector3::Zero;					//�v���C���[�̈ړ���
		Vector3 m_prevPosition = Vector3::Zero;					//1�t���[���O�̃v���C���[�̍��W
		Vector3 m_playerDirection = Vector3::Front;				//�v���C���[�̌���
		Quaternion m_toMoveDirectionRot = Quaternion::Identity;	//�v���C���[�̈ړ������ւ̃N�H�[�^�j�I��
		int m_aerialFrame = 0;									//�󒆂ɂ���t���[����
		//������΂��ړ��p�ϐ�
		int m_knockDownFrame = 0;								//�������ł���t���[����
		Vector3 m_knockDownAmount = Vector3::Zero;				//������΂���
		//�~�T�C���W�����v�p�ϐ�
		bool m_isMissileMove = false;							//�~�T�C���ړ�������?
		Vector3 m_targetPos[5] =								//�~�T�C���ړ��̃^�[�Q�b�g�̍��W
		{ Vector3::Zero,Vector3::Zero,Vector3::Zero ,Vector3::Zero,Vector3::Zero };
		int m_moveState = 0;									//�~�T�C���ړ��̏��	
		int m_targetCount = 0;									//���Ԗڂ̃~�T�C�����^�[�Q�b�g���Ă��邩
		int m_jumpFrameCount = 0;								//�W�����v���Ă���t���[����
		int m_distanceCount = 0;								//�����̃J�E���g
		bool m_isMovingUp = true;								//������Ɉړ���?
		bool m_isMoveStartFrame = true;							//�ړ��̊J�n�t���[��?
		//�{�X�O�W�����v�p�ϐ�
		bool m_isFrontMove = false;								//�{�X�O�W�����v��?
		bool m_isJumpStartFrame = true;							//�ړ��̊J�n�t���[��?
		Vector3 m_frontMoveAmount = Vector3::Zero;				//�{�X�O�W�����v�̈ړ���
		//��]�p�ϐ�
		int backHandspringFrame = 0;							//��]�̃t���[����
		Vector3 backHandSpringAmount = Vector3::Zero;			//��]�̈ړ���

		//�ړ����
		enum MoveEvent
		{
			enNormal,			//�ʏ�ړ�
			enMissileMove,		//�~�T�C���W�����v
			enFrontMove,		//�{�X�O�W�����v
			enBackHandspring,	//��]
			enKnockDown,		//������΂�
			enDead,				//���S��
		};

		MoveEvent m_playerMoveEvent = enNormal;					//���݂̈ړ����
	};
}

