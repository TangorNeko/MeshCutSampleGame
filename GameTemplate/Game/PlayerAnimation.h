#pragma once
#include "AnimationState.h"

namespace Game
{
	//プレイヤーのアニメーション一覧
	enum PlayerAnimations
	{
		enAnim_Idle,//待機
		enAnim_Walk,//歩行
		enAnim_Attack,//攻撃
		enAnim_Num//アニメーションの数
	};

	struct PlayerAnimationParam
	{
		bool isWalking = false;
		bool isAttacking = false;
		Vector3 playerPosition = Vector3::Zero;
	};

	class PlayerAnimation
	{
	public:
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
		AnimationState<PlayerAnimationParam, PlayerAnimations>* m_animationState[enAnim_Num];	//プレイヤーのアニメーションステート
		AnimationState<PlayerAnimationParam, PlayerAnimations>* m_playingAnimState;				//プレイ中のアニメーションステート
		PlayerAnimationParam m_animationParam;
		Vector3 m_prevPosition = Vector3::Zero;													//前フレームのプレイヤーの位置
		float m_attackingTime = 0.0f;
	};
}

