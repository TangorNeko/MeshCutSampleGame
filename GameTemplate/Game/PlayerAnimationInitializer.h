#pragma once
#include "AnimationState.h"
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerAnimationInitializer
	{
	public:
		/**
		 * @brief �v���C���[�̃A�j���[�V�����̏�����
		 * @param animationClips �v���C���[�̃A�j���[�V�����N���b�v
		 * @param animationState �v���C���[�̃A�j���[�V�����X�e�[�g
		*/
		void Init(AnimationClip animationClips[enAnim_Num], AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* animationState[enAnim_Num]);

	private:
		/**
		 * @brief �v���C���[�̃A�j���[�V�����N���b�v�̏�����
		 * @param animationClips �v���C���[�̃A�j���[�V�����N���b�v
		*/
		void InitAnimationClips(AnimationClip animationClips[enAnim_Num]);

		/**
		 * @brief �v���C���[�̃A�j���[�V�����X�e�[�g�̏�����
		 * @param animationState �v���C���[�̃A�j���[�V�����X�e�[�g
		*/
		void InitAnimationState(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* animationState[enAnim_Num]);
	};
}
