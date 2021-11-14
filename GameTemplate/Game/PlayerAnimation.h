#pragma once
#include "AnimationState.h"
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerAnimation
	{
	public:
		~PlayerAnimation();

		void InitAnim();

		void Update(SkinModelRender* playerRender);

		AnimationClip* GetAnimationClip()
		{
			return m_animationClips;
		}

		constexpr int GetAnimClipNum()
		{
			return static_cast<int>(enAnim_Num);
		}
	private:
		AnimationClip m_animationClips[enAnim_Num];												//プレイヤーのアニメーションクリップ
		AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* m_animationState[enAnim_Num];	//プレイヤーのアニメーションステート
		AnimationState<PlayerAnimationParam, PlayerAnimationEnum>* m_playingAnimState;				//プレイ中のアニメーションステート
		PlayerAnimationParam m_animationParam;
		Vector3 m_prevPosition = Vector3::Zero;													//前フレームのプレイヤーの位置
		float m_attackingTime = 0.0f;
	};
}

