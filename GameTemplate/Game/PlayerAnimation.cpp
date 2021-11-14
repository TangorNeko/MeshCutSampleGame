#include "stdafx.h"
#include "PlayerAnimation.h"
#include "PlayerAttackAnimationState.h"
#include "PlayerMoveAnimationState.h"


namespace
{
	const char* PATH_ANIM_IDLE = "Assets/animData/player/Idle_St1.tka";
	const char* PATH_ANIM_WALK = "Assets/animData/player/Walk_St1.tka";
	const char* PATH_ANIM_ATTACK = "Assets/animData/player/Combo1_1_St1.tka";
	const char* PATH_ANIM_ATTACK2 = "Assets/animData/player/Combo1_2_St1.tka";
	const char* PATH_ANIM_ATTACK3 = "Assets/animData/player/Combo1_3_St1.tka";
	const float SPEED_PLAYER_ANIM = 1.0f;
}

namespace Game
{
	PlayerAnimation::~PlayerAnimation()
	{
		for (int i = 0;i < enAnim_Num;i++)
		{
			delete m_animationState[i];
		}
	}

	void PlayerAnimation::InitAnim()
	{
		//�A�j���[�V�����̃��[�h
		m_animationClips[enAnim_Idle].Load(PATH_ANIM_IDLE);
		m_animationClips[enAnim_Idle].SetLoopFlag(true);
		m_animationClips[enAnim_Walk].Load(PATH_ANIM_WALK);
		m_animationClips[enAnim_Walk].SetLoopFlag(true);
		m_animationClips[enAnim_Attack].Load(PATH_ANIM_ATTACK);
		m_animationClips[enAnim_Attack].SetLoopFlag(false);
		m_animationClips[enAnim_Attack2].Load(PATH_ANIM_ATTACK2);
		m_animationClips[enAnim_Attack2].SetLoopFlag(false);
		m_animationClips[enAnim_Attack3].Load(PATH_ANIM_ATTACK3);
		m_animationClips[enAnim_Attack3].SetLoopFlag(false);

		//�A�j���[�V�����X�e�[�g��������
		for (int i = 0;i<enAnim_Num;i++)
		{
			m_animationState[i] = new AnimationState<PlayerAnimationParam, PlayerAnimationEnum>;
			m_animationState[i]->SetState(static_cast<PlayerAnimationEnum>(i));
		}

		InitIdle(m_animationState[enAnim_Idle]);
		InitWalk(m_animationState[enAnim_Walk]);
		InitAttack1(m_animationState[enAnim_Attack]);
		InitAttack2(m_animationState[enAnim_Attack2]);
		InitAttack3(m_animationState[enAnim_Attack3]);
		
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
		playerRender->PlayAnimation(nextAnim,0.1f);

		//���ݍĐ����Ă���A�j���[�V�����̃X�e�[�g���i�[
		m_playingAnimState = m_animationState[nextAnim];
	}
}