#include "stdafx.h"
#include "PlayerAnimation.h"
#include "PlayerAnimationInitializer.h"


namespace
{
	const float SPEED_PLAYER_ANIM = 1.0f;			//�A�j���[�V�����̍Đ����x
	const float TIME_ANIMATION_INTERPOLATE = 0.1f;	//�A�j���[�V�����̕⊮����
}

namespace Game
{
	PlayerAnimation::~PlayerAnimation()
	{
		//�o�^����Ă���A�j���[�V�����X�e�[�g���폜
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

		//�A�j���[�V�����̌o�߃t���[���̃J�E���g
		CountAnimationFrame(nextAnim, animParam);

		//�A�j���[�V�����̍Đ�
		playerRender->PlayAnimation(nextAnim,TIME_ANIMATION_INTERPOLATE);

		//�A�j���[�V�����̍Đ����I�����Ă��邩�̃`�F�b�N
		animParam.isPlayingAnimation = playerRender->IsPlayingAnimation();

		//���ݍĐ����Ă���A�j���[�V�����̃X�e�[�g���i�[
		m_playingAnimState = m_animationState[nextAnim];
	}

	void PlayerAnimation::CountAnimationFrame(PlayerAnimationEnum nextAnim, PlayerAnimationParam& animParam)
	{
		//�ʂ̃A�j���[�V�����ɑJ�ڂ��Ă�����
		if (m_playingAnimState != m_animationState[nextAnim])
		{
			//�o�߃t���[���������Z�b�g
			animParam.animationFrame = 0;
		}
		else
		{
			//�o�߃t���[�����𑝉�
			animParam.animationFrame++;
		}
	}
}