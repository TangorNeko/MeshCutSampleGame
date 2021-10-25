#include "stdafx.h"
#include "PlayerAnimation.h"

namespace
{
	const char* PATH_ANIM_IDLE = "Assets/animData/idle.tka";
	const char* PATH_ANIM_WALK = "Assets/animData/walk.tka";
	const float SPEED_PLAYER_ANIM = 0.5f;
}

namespace Game
{
	void PlayerAnimation::InitAnim()
	{
		//�A�j���[�V�����̃��[�h
		m_animationClips[enAnim_Idle].Load(PATH_ANIM_IDLE);
		m_animationClips[enAnim_Idle].SetLoopFlag(true);
		m_animationClips[enAnim_Walk].Load(PATH_ANIM_WALK);
		m_animationClips[enAnim_Walk].SetLoopFlag(true);
	}

	void PlayerAnimation::Update(SkinModelRender* playerRender)
	{
		//�A�j���[�V�����̃X�s�[�h���Z�b�g
		playerRender->SetAnimationSpeed(SPEED_PLAYER_ANIM);

		//�O�̈ʒu�ƕς���Ă��ꍇ�����A�j���[�V�����A����ȊO�͑ҋ@�A�j���[�V����
		if (m_prevPosition.LengthSq() != playerRender->GetPosition().LengthSq())
		{
			playerRender->PlayAnimation(enAnim_Walk);
		}
		else
		{
			playerRender->PlayAnimation(enAnim_Idle);
		}

		//�Ō�Ƀv���C���[�̈ʒu���擾
		m_prevPosition = playerRender->GetPosition();
	}
}