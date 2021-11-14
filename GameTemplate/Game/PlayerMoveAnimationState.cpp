#include "stdafx.h"
#include "PlayerMoveAnimationState.h"

namespace Game
{
	void InitIdle(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				//�O�̈ʒu�ƕς���Ă��ꍇ�����A�j���[�V�����A����ȊO�͑ҋ@�A�j���[�V����
				if (param.attackNum == 1)
				{
					return enAnim_Attack;
				}

				if (param.isWalking == true)
				{
					return enAnim_Walk;
				}

				return playerAnimState->GetMyState();
			}
		);
	}

	void InitWalk(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.attackNum == 1)
				{
					return enAnim_Attack;
				}

				if (param.isWalking == false)
				{
					return enAnim_Idle;
				}

				return playerAnimState->GetMyState();
			}
		);
	}

	void InitRun(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{

	}
}