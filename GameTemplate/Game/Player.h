#pragma once
#include "PlayerMove.h"
#include "PlayerCamera.h"
#include "PlayerAnimation.h"
#include "PlayerAttack.h"
#include "PlayerCut.h"
#include "PlayerDisplay.h"

namespace Game
{
	class Player : public IGameObject
	{
		~Player();

		bool Start() override;

		void Update() override;

	public:

		/**
		 * @brief �v���C���[�̍��W��ݒ�
		 * @param position ���W
		*/
		void SetPosition(const Vector3& position)
		{
			m_position = position;
			m_playerMove.GetCharaCon().SetPosition(m_position);
		}

		/**
		 * @brief �v���C���[�̍��W���擾
		 * @return ���W 
		*/
		const Vector3& GetPosition() const
		{
			return m_position;
		}

		/**
		 * @brief �v���C���[�̌�����ݒ�
		 * @param direction ����
		*/
		void SetPlayerDirection(const Vector3& direction)
		{
			m_playerMove.SetPlayerDirection(direction);
		}

		/**
		 * @brief �v���C���[�̃L�����R�����擾
		 * @return �v���C���[�̃L�����R��
		*/
		CharacterController& GetCharaCon()
		{
			return m_playerMove.GetCharaCon();
		}

		/**
		 * @brief �v���C���[�̃J�������W���擾
		 * @return �J�������W
		*/
		const Vector3& GetCameraPosition()
		{
			return m_playerCamera.GetCameraPosition();
		}
		
		/**
		 * @brief �v���C���[�̃J���������_���擾
		 * @return �J���������_
		*/
		const Vector3& GetCameraTarget()
		{
			return m_playerCamera.GetCameraTarget();
		}

		/**
		 * @brief �v���C���[�Ƀ_���[�W��^����
		 * @param damage �_���[�W
		*/
		void Damage(int damage);

		/**
		 * @brief �v���C���[���񕜂���
		 * @param healValue �񕜗�
		*/
		void Heal(int healValue);

		/**
		 * @brief �v���C���[�𐁂���΂�
		 * @param moveAmount ������΂���
		*/
		void KnockDown(const Vector3& moveAmount)
		{
			m_playerMove.KnockDown(moveAmount);
		}

		/**
		 * @brief �v���C���[����]������
		 * @param moveAmount ��]�̈ړ���
		*/
		void BackHandSpring(const Vector3& moveAmount)
		{
			m_playerMove.BackHandSpring(moveAmount);
		}

		/**
		 * @brief �~�T�C�����шڂ鏈���̃X�^�[�g��ʒm����
		*/
		void NoticeMissileMoveStart();

		/**
		 * @brief �{�X�̐��ʂɈړ����鏈���̃X�^�[�g��ʒm����
		*/
		void NoticeFrontMoveStart();

		/**
		 * @brief �~�T�C�����шڂ鏈���̏I����ʒm����
		*/
		void NoticeMissileMoveEnd()
		{
			m_playerMove.NoticeMissileMoveEnd();
			EventCutOff();
		}

		/**
		 * @brief �{�X�̐��ʂɈړ����鏈���̏I����ʒm����
		*/
		void NoticeFrontMoveEnd()
		{
			m_playerMove.NoticeFrontMoveEnd();
			EventCutOff();
		}

		/**
		 * @brief �g�h���J�������X�^�[�g������
		*/
		void StartFinishCamera()
		{
			m_isFinishCamera = true;
		}

		/**
		 * @brief �~�T�C�����шڂ鏈���̃R�}���h�̐�����ʒm����
		*/
		void MissileMoveSuccess()
		{
			m_playerMove.MissileMoveSuccess();
		}

		/**
		 * @brief �C�x���g�p�̃J�b�g�t���O���I��
		*/
		void EventCutOn()
		{
			m_eventCut = true;
		}

		/**
		 * @brief �C�x���g�p�̃J�b�g�t���O���I�t
		*/
		void EventCutOff()
		{
			m_eventCut = false;
		}

		/**
		 * @brief �v���C���[�̗͕̑\�����X�^�[�g������
		*/
		void InitDisplay()
		{
			m_playerDisplay.Init(m_hp);
		}

		/**
		 * @brief �v���C���[�̗̑͂��擾
		 * @return �̗�
		*/
		int GetHp()
		{
			return m_hp;
		}
	private:
		SkinModelRender* m_playerModel = nullptr;	//�v���C���[�̃��f��
		Vector3 m_position = Vector3::Zero;			//�v���C���[�̍��W
		PlayerMove m_playerMove;					//�v���C���[�̈ړ��N���X
		PlayerCamera m_playerCamera;				//�v���C���[�̃J�����N���X
		PlayerAnimation m_playerAnimation;			//�v���C���[�̃A�j���[�V�����N���X
		PlayerAnimationParam m_playerAnimationParam;//�v���C���[�̃A�j���[�V��������X�e�[�^�X
		PlayerAttack m_playerAttack;				//�v���C���[�̍U���N���X
		PlayerCut m_playerCut;						//�v���C���[�̐ؒf�N���X
		PlayerDisplay m_playerDisplay;				//�v���C���[�̏��\���N���X
		int m_hp = 1000;							//�v���C���[�̗̑�
		bool m_eventCut = false;					//�v���C���[���C�x���g�ؒf���[�h����?
		bool m_isFinishCamera = false;				//�v���C���[���g�h���J��������?
		int m_finishCount = 0;						//�g�h���J�������X�^�[�g����܂ł̃t���[�����J�E���g����
	};
}

