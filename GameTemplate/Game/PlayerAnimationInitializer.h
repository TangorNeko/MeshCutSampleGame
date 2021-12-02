#pragma once
#include "AnimationState.h"
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerAnimationInitializer
	{
	public:
		/**
		 * @brief プレイヤーのアニメーションの初期化
		 * @param animationClips プレイヤーのアニメーションクリップ
		 * @param animationState プレイヤーのアニメーションステート
		*/
		void Init(AnimationClip animationClips[enAnim_Num], AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* animationState[enAnim_Num]);

	private:
		/**
		 * @brief プレイヤーのアニメーションクリップの初期化
		 * @param animationClips プレイヤーのアニメーションクリップ
		*/
		void InitAnimationClips(AnimationClip animationClips[enAnim_Num]);

		/**
		 * @brief プレイヤーのアニメーションステートの初期化
		 * @param animationState プレイヤーのアニメーションステート
		*/
		void InitAnimationState(AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* animationState[enAnim_Num]);
	};
}
