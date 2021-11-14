#include "stdafx.h"
#include "PlayerAttackAnimationState.h"

namespace Game
{
	void InitAttack1(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
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

				return playerAnimState->GetMyState();
			}
		);
	}


	void InitAttack2(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
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

				return playerAnimState->GetMyState();
			}
		);
	}


	void InitAttack3(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
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

				return playerAnimState->GetMyState();
			}
		);
	}
	

	void InitCutMode(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.isCutMode == false)
				{
					return enAnim_Idle;
				}

				return playerAnimState->GetMyState();
			}
		);
	}

}