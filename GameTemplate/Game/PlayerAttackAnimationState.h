#pragma once
#include "AnimationState.h"
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerAttackAnimationState
	{
	public:
		/**
		 * @brief 1段目の攻撃アニメーションからのアニメーション遷移
		 * @param playerAnimState プレイヤーのアニメーションステート
		*/
		void InitAttack1(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief 2段目の攻撃アニメーションからのアニメーション遷移
		 * @param playerAnimState プレイヤーのアニメーションステート
		*/
		void InitAttack2(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief 3段目の攻撃アニメーションからのアニメーション遷移
		 * @param playerAnimState プレイヤーのアニメーションステート
		*/
		void InitAttack3(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief ガードアニメーションからのアニメーション遷移
		 * @param playerAnimState プレイヤーのアニメーションステート
		*/
		void InitGuard(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief 切断の構えアニメーションからのアニメーション遷移
		 * @param playerAnimState プレイヤーのアニメーションステート
		*/
		void InitCutMode(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);
	};
}

