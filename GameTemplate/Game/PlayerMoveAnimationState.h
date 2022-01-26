#pragma once
#include "AnimationState.h"
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerMoveAnimationState
	{
	public:
		/**
		 * @brief 待機アニメーションからのアニメーション遷移
		 * @param playerAnimState プレイヤーのアニメーションステート
		*/
		void InitIdle(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief 歩きアニメーションからのアニメーション遷移
		 * @param playerAnimState プレイヤーのアニメーションステート
		*/
		void InitWalk(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief 走りアニメーションからのアニメーション遷移
		 * @param playerAnimState プレイヤーのアニメーションステート
		*/
		void InitRun(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief ノックダウンアニメーションからのアニメーション遷移
		 * @param playerAnimState プレイヤーのアニメーションステート
		*/
		void InitKnockDown(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief 起き上がりからのアニメーション遷移
		 * @param playerAnimState プレイヤーのアニメーションステート
		*/
		void InitGetUp(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief ジャンプアニメーションからのアニメーション遷移
		 * @param playerAnimState プレイヤーのアニメーションステート
		*/
		void InitJump(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief 納刀アニメーションからのアニメーション遷移
		 * @param playerAnimState プレイヤーのアニメーションステート
		*/
		void InitUnequip(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief 納刀待機アニメーションからのアニメーション遷移
		 * @param playerAnimState プレイヤーのアニメーションステート
		*/
		void InitUnequipIdle(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);

		/**
		 * @brief 後転アニメーションからのアニメーション遷移
		 * @param playerAnimState プレイヤーのアニメーションステート
		*/
		void InitBackHandspring(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* playerAnimState);
	};
}
