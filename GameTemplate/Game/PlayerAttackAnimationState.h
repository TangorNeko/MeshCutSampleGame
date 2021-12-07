#pragma once
#include "AnimationState.h"
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerAttackAnimationState
	{
	public:
		void InitAttack1(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		void InitAttack2(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		void InitAttack3(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		void InitGuard(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		void InitCutMode(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);
	};
}

