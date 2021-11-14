#pragma once
#include "AnimationState.h"
#include "PlayerAnimationParam.h"

namespace Game
{
	void InitIdle(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

	void InitWalk(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

	void InitRun(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);
}
