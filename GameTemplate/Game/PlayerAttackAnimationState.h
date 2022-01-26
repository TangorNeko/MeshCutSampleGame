#pragma once
#include "AnimationState.h"
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerAttackAnimationState
	{
	public:
		/**
		 * @brief 1�i�ڂ̍U���A�j���[�V��������̃A�j���[�V�����J��
		 * @param playerAnimState �v���C���[�̃A�j���[�V�����X�e�[�g
		*/
		void InitAttack1(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief 2�i�ڂ̍U���A�j���[�V��������̃A�j���[�V�����J��
		 * @param playerAnimState �v���C���[�̃A�j���[�V�����X�e�[�g
		*/
		void InitAttack2(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief 3�i�ڂ̍U���A�j���[�V��������̃A�j���[�V�����J��
		 * @param playerAnimState �v���C���[�̃A�j���[�V�����X�e�[�g
		*/
		void InitAttack3(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief �K�[�h�A�j���[�V��������̃A�j���[�V�����J��
		 * @param playerAnimState �v���C���[�̃A�j���[�V�����X�e�[�g
		*/
		void InitGuard(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief �ؒf�̍\���A�j���[�V��������̃A�j���[�V�����J��
		 * @param playerAnimState �v���C���[�̃A�j���[�V�����X�e�[�g
		*/
		void InitCutMode(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);
	};
}

