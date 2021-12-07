#pragma once
#include "AnimationState.h"
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerMoveAnimationState
	{
	public:
		void InitIdle(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		void InitWalk(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		void InitRun(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		void InitKnockDown(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		void InitGetUp(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);
	};
}
