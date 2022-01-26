#include "stdafx.h"
#include "PlayerAnimation.h"
#include "PlayerAnimationInitializer.h"


namespace
{
	const float SPEED_PLAYER_ANIM = 1.0f;
	const float TIME_ANIMATION_INTERPOLATE = 0.1f;
}

namespace Game
{
	PlayerAnimation::~PlayerAnimation()
	{
		for (int animationNumber = 0;animationNumber < enAnim_Num;animationNumber++)
		{
			delete m_animationState[animationNumber];
		}
	}

	void PlayerAnimation::InitAnim()
	{
		//アニメーションを初期化
		PlayerAnimationInitializer animationInitializer;
		animationInitializer.Init(m_animationClips,m_animationState);
		
		//初期状態で再生されるアニメーションのステートを格納
		m_playingAnimState = m_animationState[enAnim_Idle];
	}

	void PlayerAnimation::Update(SkinModelRender* playerRender,PlayerAnimationParam& animParam)
	{
		//アニメーションのスピードをセット
		playerRender->SetAnimationSpeed(SPEED_PLAYER_ANIM);

		//アニメーションの遷移のチェック
		PlayerAnimationEnum nextAnim = m_playingAnimState->StateChangeCheck(animParam);

		//アニメーションの経過フレームのカウント
		CountAnimationFrame(nextAnim, animParam);

		//アニメーションの再生
		playerRender->PlayAnimation(nextAnim,TIME_ANIMATION_INTERPOLATE);

		//アニメーションの再生が終了しているかのチェック
		animParam.isPlayingAnimation = playerRender->IsPlayingAnimation();

		//現在再生しているアニメーションのステートを格納
		m_playingAnimState = m_animationState[nextAnim];
	}

	void PlayerAnimation::CountAnimationFrame(PlayerAnimationEnum nextAnim, PlayerAnimationParam& animParam)
	{
		//別のアニメーションに遷移していたら
		if (m_playingAnimState != m_animationState[nextAnim])
		{
			//経過フレーム数をリセット
			animParam.animationFrame = 0;
		}
		else
		{
			//経過フレーム数を増加
			animParam.animationFrame++;
		}
	}
}