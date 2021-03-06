#include "stdafx.h"
#include "PlayerAnimationInitializer.h"
#include "PlayerAttackAnimationState.h"
#include "PlayerMoveAnimationState.h"

namespace
{
	const char* PATH_ANIM_IDLE = "Assets/animData/player/Idle_St1.tka";						//待機アニメーションのパス
	const char* PATH_ANIM_WALK = "Assets/animData/player/Walk_St1.tka";						//歩きアニメーションのパス
	const char* PATH_ANIM_RUN = "Assets/animData/player/Run_St1.tka";						//走りアニメーションのパス
	const char* PATH_ANIM_ATTACK = "Assets/animData/player/Combo1_1_St1.tka";				//コンボ1アニメーションのパス
	const char* PATH_ANIM_ATTACK2 = "Assets/animData/player/Combo1_2_St1.tka";				//コンボ2アニメーションのパス
	const char* PATH_ANIM_ATTACK3 = "Assets/animData/player/Combo1_3_St1.tka";				//コンボ3アニメーションのパス
	const char* PATH_ANIM_GUARD = "Assets/animData/player/Guard.tka";						//ガードアニメーションのパス
	const char* PATH_ANIM_CUTMODE = "Assets/animData/player/CutPose.tka";					//切断モードアニメーションのパス
	const char* PATH_ANIM_KNOCKDOWN = "Assets/animData/player/Hit_KnockDown.tka";			//吹き飛ばしアニメーションのパス
	const char* PATH_ANIM_GETUP = "Assets/animData/player/Getup_2.tka";						//起き上がりアニメーションのパス
	const char* PATH_ANIM_JUMP = "Assets/animData/player/Jump.tka";							//ジャンプアニメーションのパス
	const char* PATH_ANIM_UNEQUIP = "Assets/animData/player/Unequip_St1.tka";				//納刀アニメーションのパス
	const char* PATH_ANIM_UNEQUIP_IDLE = "Assets/animData/player/Idle_Unequip.tka";			//納刀待機アニメーションのパス
	const char* PATH_ANIM_BACKHANDSPRING = "Assets/animData/player/BackHandspring.tka";		//後転アニメーションのパス
	const char* PATH_ANIM_DEAD = "Assets/animData/player/Dead.tka";							//死亡アニメーションのパス
}

namespace Game
{
	void PlayerAnimationInitializer::Init(AnimationClip animationClips[enAnim_Num], AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* animationState[enAnim_Num])
	{
		//アニメーションクリップの初期化
		InitAnimationClips(animationClips);

		//アニメーションステートの初期化
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
		animationClips[enAnim_Jump].Load(PATH_ANIM_JUMP);
		animationClips[enAnim_Jump].SetLoopFlag(false);
		animationClips[enAnim_Unequip].Load(PATH_ANIM_UNEQUIP);
		animationClips[enAnim_Unequip].SetLoopFlag(false);
		animationClips[enAnim_Idle_Unequip].Load(PATH_ANIM_UNEQUIP_IDLE);
		animationClips[enAnim_Idle_Unequip].SetLoopFlag(true);
		animationClips[enAnim_BackHandspring].Load(PATH_ANIM_BACKHANDSPRING);
		animationClips[enAnim_BackHandspring].SetLoopFlag(false);
		animationClips[enAnim_Dead].Load(PATH_ANIM_DEAD);
		animationClips[enAnim_Dead].SetLoopFlag(false);
	}

	void PlayerAnimationInitializer::InitAnimationState(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* animationState[enAnim_Num])
	{
		//アニメーションステートを初期化
		for (int animationNumber = 0;animationNumber < enAnim_Num;animationNumber++)
		{
			//プレイヤーのアニメーションパラメーターに合わせたアニメーションステートを作成
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
		moveAnimationState.InitJump(animationState[enAnim_Jump]);
		moveAnimationState.InitUnequip(animationState[enAnim_Unequip]);
		moveAnimationState.InitUnequipIdle(animationState[enAnim_Idle_Unequip]);
		moveAnimationState.InitBackHandspring(animationState[enAnim_BackHandspring]);
		moveAnimationState.InitDead(animationState[enAnim_Dead]);

		PlayerAttackAnimationState attackAnimationState;
		attackAnimationState.InitAttack1(animationState[enAnim_Attack]);
		attackAnimationState.InitAttack2(animationState[enAnim_Attack2]);
		attackAnimationState.InitAttack3(animationState[enAnim_Attack3]);
		attackAnimationState.InitGuard(animationState[enAnim_Guard]);
		attackAnimationState.InitCutMode(animationState[enAnim_CutMode]);
	}
}
