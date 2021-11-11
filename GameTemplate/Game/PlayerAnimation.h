#pragma once
#include "AnimationState.h"

namespace Game
{
	//�v���C���[�̃A�j���[�V�����ꗗ
	enum PlayerAnimations
	{
		enAnim_Idle,//�ҋ@
		enAnim_Walk,//���s
		enAnim_Attack,//�U��
		enAnim_Num//�A�j���[�V�����̐�
	};

	struct PlayerAnimationParam
	{
		bool isWalking = false;
		bool isAttacking = false;
		Vector3 playerPosition = Vector3::Zero;
	};

	class PlayerAnimation
	{
	public:
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
		AnimationState<PlayerAnimationParam, PlayerAnimations>* m_animationState[enAnim_Num];	//�v���C���[�̃A�j���[�V�����X�e�[�g
		AnimationState<PlayerAnimationParam, PlayerAnimations>* m_playingAnimState;				//�v���C���̃A�j���[�V�����X�e�[�g
		PlayerAnimationParam m_animationParam;
		Vector3 m_prevPosition = Vector3::Zero;													//�O�t���[���̃v���C���[�̈ʒu
		float m_attackingTime = 0.0f;
	};
}

