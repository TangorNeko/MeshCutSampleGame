#pragma once
#include "AnimationState.h"
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerAnimation
	{
	public:
		~PlayerAnimation();

		void InitAnim();

		void Update(SkinModelRender* playerRender);

		AnimationClip* GetAnimationClip()
		{
			return m_animationClips;
		}

		constexpr int GetAnimClipNum()
		{
			return static_cast<int>(enAnim_Num);
		}
	private:
		AnimationClip m_animationClips[enAnim_Num];												//�v���C���[�̃A�j���[�V�����N���b�v
		AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* m_animationState[enAnim_Num];	//�v���C���[�̃A�j���[�V�����X�e�[�g
		AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* m_playingAnimState;				//�v���C���̃A�j���[�V�����X�e�[�g
		PlayerAnimationParam m_animationParam;
		Vector3 m_prevPosition = Vector3::Zero;													//�O�t���[���̃v���C���[�̈ʒu
		float m_attackingTime = 0.0f;
	};
}

