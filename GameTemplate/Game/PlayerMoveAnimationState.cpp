#include "stdafx.h"
#include "PlayerMoveAnimationState.h"

namespace Game
{
	void PlayerMoveAnimationState::InitIdle(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				//�O�̈ʒu�ƕς���Ă��ꍇ�����A�j���[�V�����A����ȊO�͑ҋ@�A�j���[�V����
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

				if (param.isKnockDown == true)
				{
					return enAnim_KnockDown;
				}

				return playerAnimState->GetMyState();
			}
		);
	}

	void PlayerMoveAnimationState::InitWalk(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
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

				if (param.isKnockDown == true)
				{
					return enAnim_KnockDown;
				}

				return playerAnimState->GetMyState();
			}
		);
	}

	void PlayerMoveAnimationState::InitRun(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
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

				if (param.isKnockDown == true)
				{
					return enAnim_KnockDown;
				}

				return playerAnimState->GetMyState();
			}
		);
	}

	void PlayerMoveAnimationState::InitKnockDown(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.downTime >= 100)
				{
					return enAnim_GetUp;
				}

				return playerAnimState->GetMyState();
			}
		);
	}
	
	void PlayerMoveAnimationState::InitGetUp(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.downTime >= 150)
				{
					return enAnim_Idle;
				}

				return playerAnimState->GetMyState();
			}
		);
	}
}