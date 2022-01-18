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

				if (param.isCutMode == true)
				{
					return enAnim_CutMode;
				}

				if (param.isKnockDown == true)
				{
					return enAnim_KnockDown;
				}

				if (param.isGuarding == true)
				{
					return enAnim_Guard;
				}

				if (param.isJumping == true)
				{
					return enAnim_Jump;
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

				if (param.isCutMode == true)
				{
					return enAnim_CutMode;
				}

				if (param.isKnockDown == true)
				{
					return enAnim_KnockDown;
				}

				if (param.isGuarding == true)
				{
					return enAnim_Guard;
				}

				if (param.isJumping == true)
				{
					return enAnim_Jump;
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

				if (param.isCutMode == true)
				{
					return enAnim_CutMode;
				}

				if (param.isKnockDown == true)
				{
					return enAnim_KnockDown;
				}

				if (param.isGuarding == true)
				{
					return enAnim_Guard;
				}

				if (param.isJumping == true)
				{
					return enAnim_Jump;
				}

				return playerAnimState->GetMyState();
			}
		);
	}
	

	void PlayerAttackAnimationState::InitGuard(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.isGuarding == false)
				{
					return enAnim_Idle;
				}

				return playerAnimState->GetMyState();
			}
		);
	}

	void PlayerAttackAnimationState::InitCutMode(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.isCutMode == false)
				{
					return enAnim_Idle;
				}

				if (param.isKnockDown == true)
				{
					return enAnim_KnockDown;
				}

				return playerAnimState->GetMyState();
			}
		);
	}

}