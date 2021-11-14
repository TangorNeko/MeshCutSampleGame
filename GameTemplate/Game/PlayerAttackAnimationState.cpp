#include "stdafx.h"
#include "PlayerAttackAnimationState.h"

namespace Game
{
	void InitAttack1(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.attackNum == 0)
				{
					return enAnim_Idle;
				}

				if (param.attackingTime == 0.0f)
				{
					return enAnim_Attack2;
				}

				return playerAnimState->GetMyState();
			}
		);
	}


	void InitAttack2(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.attackNum == 0)
				{
					return enAnim_Idle;
				}

				if (param.attackingTime == 0.0f)
				{
					return enAnim_Attack3;
				}

				return playerAnimState->GetMyState();
			}
		);
	}


	void InitAttack3(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.attackNum == 0)
				{
					return enAnim_Idle;
				}

				return playerAnimState->GetMyState();
			}
		);
	}

}