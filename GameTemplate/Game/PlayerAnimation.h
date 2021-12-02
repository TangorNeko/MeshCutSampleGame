#pragma once
#include "AnimationState.h"
#include "PlayerAnimationParam.h"

namespace Game
{
	using PlayerAnimationState = AnimationState<PlayerAnimationParam, PlayerAnimationEnum>;

	class PlayerAnimation
	{
	public:
		~PlayerAnimation();

		/**
		 * @brief アニメーションの初期化
		*/
		void InitAnim();

		/**
		 * @brief アニメーションのアップデート
		 * @param playerRender プレイヤーのスキンモデルレンダー
		 * @param animParam プレイヤーのアニメーションパラメーター
		*/
		void Update(SkinModelRender* playerRender, PlayerAnimationParam& animParam);

		/**
		 * @brief アニメーションクリップを取得
		 * @return アニメーションクリップ
		*/
		AnimationClip* GetAnimationClip()
		{
			return m_animationClips;
		}

		/**
		 * @brief アニメーションクリップの数を取得
		 * @return アニメーションクリップの数
		*/
		constexpr int GetAnimClipNum()
		{
			return static_cast<int>(enAnim_Num);
		}

	private:
		AnimationClip m_animationClips[enAnim_Num];				//プレイヤーのアニメーションクリップ
		PlayerAnimationState* m_animationState[enAnim_Num];		//プレイヤーのアニメーションステート
		PlayerAnimationState* m_playingAnimState;				//再生中のアニメーションのアニメーションステート
	};
}

