#include "stdafx.h"
#include "PlayerAnimation.h"

namespace
{
	const char* PATH_ANIM_IDLE = "Assets/animData/idle.tka";
	const char* PATH_ANIM_WALK = "Assets/animData/walk.tka";
	const float SPEED_PLAYER_ANIM = 0.5f;
}

namespace Game
{
	void PlayerAnimation::InitAnim()
	{
		//アニメーションのロード
		m_animationClips[enAnim_Idle].Load(PATH_ANIM_IDLE);
		m_animationClips[enAnim_Idle].SetLoopFlag(true);
		m_animationClips[enAnim_Walk].Load(PATH_ANIM_WALK);
		m_animationClips[enAnim_Walk].SetLoopFlag(true);
	}

	void PlayerAnimation::Update(SkinModelRender* playerRender)
	{
		//アニメーションのスピードをセット
		playerRender->SetAnimationSpeed(SPEED_PLAYER_ANIM);

		//前の位置と変わってた場合歩きアニメーション、それ以外は待機アニメーション
		if (m_prevPosition.LengthSq() != playerRender->GetPosition().LengthSq())
		{
			playerRender->PlayAnimation(enAnim_Walk);
		}
		else
		{
			playerRender->PlayAnimation(enAnim_Idle);
		}

		//最後にプレイヤーの位置を取得
		m_prevPosition = playerRender->GetPosition();
	}
}