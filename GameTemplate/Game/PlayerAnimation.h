#pragma once
#include "AnimationState.h"
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerAnimation
	{
	public:
		~PlayerAnimation();

		/**
		 * @brief �A�j���[�V�����̏�����
		*/
		void InitAnim();

		/**
		 * @brief �A�j���[�V�����̃A�b�v�f�[�g
		 * @param playerRender �v���C���[�̃X�L�����f�������_�[
		 * @param animParam �v���C���[�̃A�j���[�V�����p�����[�^�[
		*/
		void Update(SkinModelRender* playerRender, PlayerAnimationParam& animParam);

		/**
		 * @brief �A�j���[�V�����N���b�v���擾
		 * @return �A�j���[�V�����N���b�v
		*/
		AnimationClip* GetAnimationClip()
		{
			return m_animationClips;
		}

		/**
		 * @brief �A�j���[�V�����N���b�v�̐����擾
		 * @return �A�j���[�V�����N���b�v�̐�
		*/
		constexpr int GetAnimClipNum()
		{
			return static_cast<int>(enAnim_Num);
		}

	private:
		AnimationClip m_animationClips[enAnim_Num];												//�v���C���[�̃A�j���[�V�����N���b�v
		AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* m_animationState[enAnim_Num];//�v���C���[�̃A�j���[�V�����X�e�[�g
		AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* m_playingAnimState;			//�Đ����̃A�j���[�V�����̃A�j���[�V�����X�e�[�g
		float m_attackingTime = 0.0f;
	};
}

