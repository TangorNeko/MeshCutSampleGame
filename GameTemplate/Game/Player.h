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

		void SetPosition(const Vector3& position)
		{
			m_position = position;
			m_playerMove.GetCharaCon().SetPosition(m_position);
		}

		const Vector3& GetPosition() const
		{
			return m_position;
		}

		CharacterController& GetCharaCon()
		{
			return m_playerMove.GetCharaCon();
		}

		const Vector3& GetCameraPosition()
		{
			return m_playerCamera.GetCameraPosition();
		}
		
		const Vector3& GetCameraTarget()
		{
			return m_playerCamera.GetCameraTarget();
		}

		void Damage(int damage);

		void Heal(int healValue);

		void KnockDown(const Vector3& moveAmount)
		{
			m_playerMove.KnockDown(moveAmount);
		}

		void BackHandSpring(const Vector3& moveAmount)
		{
			m_playerMove.BackHandSpring(moveAmount);
		}

		void NoticeMissileMoveEnd()
		{
			m_playerMove.NoticeMissileMoveEnd();
			m_eventCut = false;
		}

		void NoticeFrontMoveEnd()
		{
			m_playerMove.NoticeFrontMoveEnd();
			m_eventCut = false;
		}

		//TODO:���ł��B�K�[�h�͐�p�֐������A
		//�A�j���[�V�����p�����[�^�[�͒��ڎg�������Ȃ��B
		bool isGuard()
		{
			return m_playerAnimationParam.isGuarding;
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
		int m_hp = 1000;
		bool m_eventCut = false;
		FontRender* m_hpRender = nullptr;
	};
}

