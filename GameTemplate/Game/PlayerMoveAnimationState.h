#pragma once
#include "AnimationState.h"
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerMoveAnimationState
	{
	public:
		/**
		 * @brief �ҋ@�A�j���[�V��������̃A�j���[�V�����J��
		 * @param playerAnimState �v���C���[�̃A�j���[�V�����X�e�[�g
		*/
		void InitIdle(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief �����A�j���[�V��������̃A�j���[�V�����J��
		 * @param playerAnimState �v���C���[�̃A�j���[�V�����X�e�[�g
		*/
		void InitWalk(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief ����A�j���[�V��������̃A�j���[�V�����J��
		 * @param playerAnimState �v���C���[�̃A�j���[�V�����X�e�[�g
		*/
		void InitRun(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief �m�b�N�_�E���A�j���[�V��������̃A�j���[�V�����J��
		 * @param playerAnimState �v���C���[�̃A�j���[�V�����X�e�[�g
		*/
		void InitKnockDown(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief �N���オ�肩��̃A�j���[�V�����J��
		 * @param playerAnimState �v���C���[�̃A�j���[�V�����X�e�[�g
		*/
		void InitGetUp(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief �W�����v�A�j���[�V��������̃A�j���[�V�����J��
		 * @param playerAnimState �v���C���[�̃A�j���[�V�����X�e�[�g
		*/
		void InitJump(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief �[���A�j���[�V��������̃A�j���[�V�����J��
		 * @param playerAnimState �v���C���[�̃A�j���[�V�����X�e�[�g
		*/
		void InitUnequip(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief �[���ҋ@�A�j���[�V��������̃A�j���[�V�����J��
		 * @param playerAnimState �v���C���[�̃A�j���[�V�����X�e�[�g
		*/
		void InitUnequipIdle(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief ��]�A�j���[�V��������̃A�j���[�V�����J��
		 * @param playerAnimState �v���C���[�̃A�j���[�V�����X�e�[�g
		*/
		void InitBackHandspring(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);
	};
}
