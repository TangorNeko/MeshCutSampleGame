#include "stdafx.h"
#include "PlayerMoveAnimationState.h"

namespace Game
{
	void InitIdle(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				//前の位置と変わってた場合歩きアニメーション、それ以外は待機アニメーション
				if (param.comboNum == 1)
				{
					return enAnim_Attack;
				}

				if (param.isRunning == true)
				{
					return enAnim_Run;
				}

				if (param.isWalking == true)
				{
					return enAnim_Walk;
				}

				if (param.isCutMode == true)
				{
					return enAnim_CutMode;
				}

				if (param.isGuarding == true)
				{
					return enAnim_Guard;
				}

				return playerAnimState->GetMyState();
			}
		);
	}

	void InitWalk(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.comboNum == 1)
				{
					return enAnim_Attack;
				}

				if (param.isRunning == true)
				{
					return enAnim_Run;
				}

				if (param.isWalking == false)
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

	void InitRun(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.comboNum == 1)
				{
					return enAnim_Attack;
				}

				if (param.isWalking == true)
				{
					return enAnim_Walk;
				}

				if (param.isRunning == false)
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
}