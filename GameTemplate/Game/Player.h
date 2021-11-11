#pragma once
#include "PlayerMove.h"
#include "PlayerCamera.h"
#include "PlayerAnimation.h"
#include "PlayerAttack.h"
#include "PlayerCut.h"

namespace Game
{
	class Player : public IGameObject
	{
		~Player();

		bool Start() override;

		void Update() override;

	private:
		SkinModelRender* m_playerModel = nullptr;	//�v���C���[�̃��f��
		Vector3 m_position = Vector3::Zero;			//�v���C���[�̍��W
		PlayerMove m_playerMove;					//�v���C���[�̈ړ��N���X
		PlayerCamera m_playerCamera;				//�v���C���[�̃J�����N���X
		PlayerAnimation m_playerAnimation;			//�v���C���[�̃A�j���[�V�����N���X
		PlayerAnimationParam m_playerAnimationParam;//�v���C���[�̃A�j���[�V��������X�e�[�^�X
		PlayerAttack m_playerAttack;				//�v���C���[�̍U���N���X
		PlayerCut m_playerCut;						//�v���C���[�̐ؒf�N���X
	};
}

