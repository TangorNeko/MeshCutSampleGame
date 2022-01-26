#include "stdafx.h"
#include "PlayerAttackAnimationState.h"

namespace Game
{
	void PlayerAttackAnimationState::InitAttack1(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.comboNum == 0)
				{
					return enAnim_Idle;
				}

				if (param.attackingTime == 0)
				{
					return enAnim_Attack2;
				}

				if(param.playerState == PlayerAnimationParam::enCutMode)
				{
					return enAnim_CutMode;
				}

				if(param.playerState == PlayerAnimationParam::enKnockDown)
				{
					return enAnim_KnockDown;
				}

				if(param.playerState == PlayerAnimationParam::enGuard)
				{
					return enAnim_Guard;
				}

				if(param.playerState == PlayerAnimationParam::enJumping)
				{
					return enAnim_Jump;
				}

				if (param.playerState == PlayerAnimationParam::enDead)
				{
					return enAnim_Dead;
				}

				return playerAnimState->GetMyState();
			}
		);
	}


	void PlayerAttackAnimationState::InitAttack2(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.comboNum == 0)
				{
					return enAnim_Idle;
				}

				if (param.attackingTime == 0)
				{
					return enAnim_Attack3;
				}

				if(param.playerState == PlayerAnimationParam::enCutMode)
				{
					return enAnim_CutMode;
				}

				if(param.playerState == PlayerAnimationParam::enKnockDown)
				{
					return enAnim_KnockDown;
				}

				if(param.playerState == PlayerAnimationParam::enGuard)
				{
					return enAnim_Guard;
				}

				if(param.playerState == PlayerAnimationParam::enJumping)
				{
					return enAnim_Jump;
				}

				if (param.playerState == PlayerAnimationParam::enDead)
				{
					return enAnim_Dead;
				}

				return playerAnimState->GetMyState();
			}
		);
	}


	void PlayerAttackAnimationState::InitAttack3(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.comboNum == 0)
				{
					return enAnim_Idle;
				}

				if(param.playerState == PlayerAnimationParam::enCutMode)
				{
					return enAnim_CutMode;
				}

				if(param.playerState == PlayerAnimationParam::enKnockDown)
				{
					return enAnim_KnockDown;
				}

				if(param.playerState == PlayerAnimationParam::enGuard)
				{
					return enAnim_Guard;
				}

				if(param.playerState == PlayerAnimationParam::enJumping)
				{
					return enAnim_Jump;
				}

				if (param.playerState == PlayerAnimationParam::enDead)
				{
					return enAnim_Dead;
				}

				return playerAnimState->GetMyState();
			}
		);
	}
	

	void PlayerAttackAnimationState::InitGuard(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if(param.playerState == PlayerAnimationParam::enIdle)
				{
					return enAnim_Idle;
				}

				if (param.playerState == PlayerAnimationParam::enDead)
				{
					return enAnim_Dead;
				}

				return playerAnimState->GetMyState();
			}
		);
	}

	void PlayerAttackAnimationState::InitCutMode(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if(param.playerState == PlayerAnimationParam::enIdle)
				{
					return enAnim_Idle;
				}

				if(param.playerState == PlayerAnimationParam::enKnockDown)
				{
					return enAnim_KnockDown;
				}

				if (param.playerState == PlayerAnimationParam::enBackHandSpring)
				{
					return enAnim_BackHandspring;
				}

				if (param.playerState == PlayerAnimationParam::enDead)
				{
					return enAnim_Dead;
				}

				return playerAnimState->GetMyState();
			}
		);
	}

}