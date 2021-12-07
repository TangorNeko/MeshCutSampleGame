#include "stdafx.h"
#include "PlayerAnimationInitializer.h"
#include "PlayerAttackAnimationState.h"
#include "PlayerMoveAnimationState.h"

namespace
{
	const char* PATH_ANIM_IDLE = "Assets/animData/player/Idle_St1.tka";
	const char* PATH_ANIM_WALK = "Assets/animData/player/Walk_St1.tka";
	const char* PATH_ANIM_RUN = "Assets/animData/player/Run_St1.tka";
	const char* PATH_ANIM_ATTACK = "Assets/animData/player/Combo1_1_St1.tka";
	const char* PATH_ANIM_ATTACK2 = "Assets/animData/player/Combo1_2_St1.tka";
	const char* PATH_ANIM_ATTACK3 = "Assets/animData/player/Combo1_3_St1.tka";
	const char* PATH_ANIM_GUARD = "Assets/animData/player/Guard.tka";
	const char* PATH_ANIM_CUTMODE = "Assets/animData/player/Idle_St1.tka";
	const char* PATH_ANIM_KNOCKDOWN = "Assets/animData/player/Hit_KnockDown.tka";
	const char* PATH_ANIM_GETUP = "Assets/animData/player/Getup_2.tka";
}

namespace Game
{
	void PlayerAnimationInitializer::Init(AnimationClip animationClips[enAnim_Num], AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* animationState[enAnim_Num])
	{
		InitAnimationClips(animationClips);

		InitAnimationState(animationState);
	}

	void PlayerAnimationInitializer::InitAnimationClips(AnimationClip animationClips[enAnim_Num])
	{
		//アニメーションのロード
		animationClips[enAnim_Idle].Load(PATH_ANIM_IDLE);
		animationClips[enAnim_Idle].SetLoopFlag(true);
		animationClips[enAnim_Walk].Load(PATH_ANIM_WALK);
		animationClips[enAnim_Walk].SetLoopFlag(true);
		animationClips[enAnim_Run].Load(PATH_ANIM_RUN);
		animationClips[enAnim_Run].SetLoopFlag(true);
		animationClips[enAnim_Attack].Load(PATH_ANIM_ATTACK);
		animationClips[enAnim_Attack].SetLoopFlag(false);
		animationClips[enAnim_Attack2].Load(PATH_ANIM_ATTACK2);
		animationClips[enAnim_Attack2].SetLoopFlag(false);
		animationClips[enAnim_Attack3].Load(PATH_ANIM_ATTACK3);
		animationClips[enAnim_Attack3].SetLoopFlag(false);
		animationClips[enAnim_Guard].Load(PATH_ANIM_GUARD);
		animationClips[enAnim_Guard].SetLoopFlag(true);
		animationClips[enAnim_CutMode].Load(PATH_ANIM_CUTMODE);
		animationClips[enAnim_CutMode].SetLoopFlag(true);
		animationClips[enAnim_KnockDown].Load(PATH_ANIM_KNOCKDOWN);
		animationClips[enAnim_KnockDown].SetLoopFlag(false);
		animationClips[enAnim_GetUp].Load(PATH_ANIM_GETUP);
		animationClips[enAnim_GetUp].SetLoopFlag(false);
	}

	void PlayerAnimationInitializer::InitAnimationState(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* animationState[enAnim_Num])
	{
		//アニメーションステートを初期化
		for (int animationNumber = 0;animationNumber < enAnim_Num;animationNumber++)
		{
			animationState[animationNumber] = new AnimationState<PlayerAnimationParam, PlayerAnimationEnum>;
			
			//自分がどのアニメーションのステートなのかを設定
			animationState[animationNumber]->SetState(static_cast<PlayerAnimationEnum>(animationNumber));
		}

		//各アニメーションステートの遷移条件をセット
		PlayerMoveAnimationState moveAnimationState;
		moveAnimationState.InitIdle(animationState[enAnim_Idle]);
		moveAnimationState.InitWalk(animationState[enAnim_Walk]);
		moveAnimationState.InitRun(animationState[enAnim_Run]);
		moveAnimationState.InitKnockDown(animationState[enAnim_KnockDown]);
		moveAnimationState.InitGetUp(animationState[enAnim_GetUp]);

		PlayerAttackAnimationState attackAnimationState;
		attackAnimationState.InitAttack1(animationState[enAnim_Attack]);
		attackAnimationState.InitAttack2(animationState[enAnim_Attack2]);
		attackAnimationState.InitAttack3(animationState[enAnim_Attack3]);
		attackAnimationState.InitGuard(animationState[enAnim_Guard]);
		attackAnimationState.InitCutMode(animationState[enAnim_CutMode]);
	}
}
