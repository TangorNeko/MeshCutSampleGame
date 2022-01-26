#include "stdafx.h"
#include "PlayerMoveAnimationState.h"

namespace Game
{
	void PlayerMoveAnimationState::InitIdle(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				//前の位置と変わってた場合歩きアニメーション、それ以外は待機アニメーション
				if (param.comboNum == 1)
				{
					return enAnim_Attack;
				}

				if(param.playerState == PlayerAnimationParam::enRunning)
				{
					return enAnim_Run;
				}

				if(param.playerState == PlayerAnimationParam::enWalk)
				{
					return enAnim_Walk;
				}

				if(param.playerState == PlayerAnimationParam::enCutMode)
				{
					return enAnim_CutMode;
				}

				if(param.playerState == PlayerAnimationParam::enGuard)
				{
					return enAnim_Guard;
				}

				if(param.playerState == PlayerAnimationParam::enKnockDown)
				{
					return enAnim_KnockDown;
				}

				if(param.playerState == PlayerAnimationParam::enJumping)
				{
					return enAnim_Jump;
				}

				if(param.playerState == PlayerAnimationParam::enUnequip)
				{
					return enAnim_Unequip;
				}
				
				if(param.playerState == PlayerAnimationParam::enBackHandSpring)
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

	void PlayerMoveAnimationState::InitWalk(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.comboNum == 1)
				{
					return enAnim_Attack;
				}

				if(param.playerState == PlayerAnimationParam::enRunning)
				{
					return enAnim_Run;
				}

				if(param.playerState == PlayerAnimationParam::enIdle)
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

				if(param.playerState == PlayerAnimationParam::enJumping)
				{
					return enAnim_Jump;
				}

				if(param.playerState == PlayerAnimationParam::enUnequip)
				{
					return enAnim_Unequip;
				}

				if(param.playerState == PlayerAnimationParam::enBackHandSpring)
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

	void PlayerMoveAnimationState::InitRun(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.comboNum == 1)
				{
					return enAnim_Attack;
				}

				if(param.playerState == PlayerAnimationParam::enWalk)
				{
					return enAnim_Walk;
				}

				if(param.playerState == PlayerAnimationParam::enIdle)
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

				if(param.playerState == PlayerAnimationParam::enJumping)
				{
					return enAnim_Jump;
				}

				if(param.playerState == PlayerAnimationParam::enUnequip)
				{
					return enAnim_Unequip;
				}

				if(param.playerState == PlayerAnimationParam::enBackHandSpring)
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

	void PlayerMoveAnimationState::InitKnockDown(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.animationFrame >= 100)
				{
					return enAnim_GetUp;
				}

				if (param.playerState == PlayerAnimationParam::enDead)
				{
					return enAnim_Dead;
				}

				return playerAnimState->GetMyState();
			}
		);
	}
	
	void PlayerMoveAnimationState::InitGetUp(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.animationFrame >= 50)
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

	void PlayerMoveAnimationState::InitJump(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if(param.playerState == PlayerAnimationParam::enIdle)
				{
					return enAnim_Idle;
				}

				if(param.playerState == PlayerAnimationParam::enCutMode)
				{
					return enAnim_CutMode;
				}

				if (param.playerState == PlayerAnimationParam::enDead)
				{
					return enAnim_Dead;
				}

				return playerAnimState->GetMyState();
			}
		);
	}

	void PlayerMoveAnimationState::InitUnequip(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.isPlayingAnimation == false)
				{
					return enAnim_Idle_Unequip;
				}

				return playerAnimState->GetMyState();
			}
		);
	}

	void PlayerMoveAnimationState::InitUnequipIdle(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				return playerAnimState->GetMyState();
			}
		);
	}

	void PlayerMoveAnimationState::InitBackHandspring(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				if (param.animationFrame == 100)
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
	
	void PlayerMoveAnimationState::InitDead(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState)
	{
		playerAnimState->AddCondition([playerAnimState](const PlayerAnimationParam& param)->PlayerAnimationEnum
			{
				return playerAnimState->GetMyState();
			}
		);
	}
}