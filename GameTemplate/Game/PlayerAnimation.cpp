#include "stdafx.h"
#include "PlayerAnimation.h"

namespace
{
	const char* PATH_ANIM_IDLE = "Assets/animData/S2_Idle.tka";
	const char* PATH_ANIM_WALK = "Assets/animData/S2_Walk.tka";
	const char* PATH_ANIM_ATTACK = "Assets/animData/S2_Attack1_3.tka";
	const float SPEED_PLAYER_ANIM = 1.0f;
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
		m_animationClips[enAnim_Attack].Load(PATH_ANIM_ATTACK);
		m_animationClips[enAnim_Attack].SetLoopFlag(false);
	}

	void PlayerAnimation::Update(SkinModelRender* playerRender)
	{
		//�A�j���[�V�����̃X�s�[�h���Z�b�g
		playerRender->SetAnimationSpeed(SPEED_PLAYER_ANIM);

		//�O�̈ʒu�ƕς���Ă��ꍇ�����A�j���[�V�����A����ȊO�͑ҋ@�A�j���[�V����
		if (g_pad[0]->IsPress(enButtonY))
		{
			playerRender->PlayAnimation(enAnim_Attack);
		}
		else if(m_prevPosition.LengthSq() != playerRender->GetPosition().LengthSq())
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