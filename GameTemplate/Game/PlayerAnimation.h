#pragma once
#include "AnimationState.h"
#include "PlayerAnimationParam.h"

namespace Game
{
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
		AnimationClip m_animationClips[enAnim_Num];												//プレイヤーのアニメーションクリップ
		AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* m_animationState[enAnim_Num];//プレイヤーのアニメーションステート
		AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* m_playingAnimState;			//再生中のアニメーションのアニメーションステート
		float m_attackingTime = 0.0f;
	};
}

