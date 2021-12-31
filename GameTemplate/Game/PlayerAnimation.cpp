#include "stdafx.h"
#include "PlayerAnimation.h"
#include "PlayerAnimationInitializer.h"


namespace
{
	const float SPEED_PLAYER_ANIM = 1.0f;
	const float TIME_ANIMATION_INTERPOLATE = 0.1f;
}

namespace Game
{
	PlayerAnimation::~PlayerAnimation()
	{
		for (int animationNumber = 0;animationNumber < enAnim_Num;animationNumber++)
		{
			delete m_animationState[animationNumber];
		}
	}

	void PlayerAnimation::InitAnim()
	{
		//�A�j���[�V������������
		PlayerAnimationInitializer animationInitializer;
		animationInitializer.Init(m_animationClips,m_animationState);
		
		//������ԂōĐ������A�j���[�V�����̃X�e�[�g���i�[
		m_playingAnimState = m_animationState[enAnim_Idle];
	}

	void PlayerAnimation::Update(SkinModelRender* playerRender,PlayerAnimationParam& animParam)
	{
		//�A�j���[�V�����̃X�s�[�h���Z�b�g
		playerRender->SetAnimationSpeed(SPEED_PLAYER_ANIM);

		//�A�j���[�V�����̑J�ڂ̃`�F�b�N
		PlayerAnimationEnum nextAnim = m_playingAnimState->StateChangeCheck(animParam);

		//�A�j���[�V�����̍Đ�
		playerRender->PlayAnimation(nextAnim,TIME_ANIMATION_INTERPOLATE);

		animParam.isPlayingAnimation = playerRender->IsPlayingAnimation();

		//���ݍĐ����Ă���A�j���[�V�����̃X�e�[�g���i�[
		m_playingAnimState = m_animationState[nextAnim];
	}
}