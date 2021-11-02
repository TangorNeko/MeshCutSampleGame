#include "stdafx.h"
#include "PlayerAnimation.h"

namespace
{
	const char* PATH_ANIM_IDLE = "Assets/animData/S2_Idle.tka";
	const char* PATH_ANIM_WALK = "Assets/animData/S2_Walk.tka";
	const char* PATH_ANIM_ATTACK = "Assets/animData/S2_Attack1_3.tka";
	const float SPEED_PLAYER_ANIM = 1.0f;
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
		m_animationClips[enAnim_Attack].Load(PATH_ANIM_ATTACK);
		m_animationClips[enAnim_Attack].SetLoopFlag(false);
	}

	void PlayerAnimation::Update(SkinModelRender* playerRender)
	{
		//アニメーションのスピードをセット
		playerRender->SetAnimationSpeed(SPEED_PLAYER_ANIM);

		//前の位置と変わってた場合歩きアニメーション、それ以外は待機アニメーション
		if (g_pad[0]->IsPress(enButtonY))
		{
			playerRender->PlayAnimation(enAnim_Attack);
		}
		else if(m_prevPosition.LengthSq() != playerRender->GetPosition().LengthSq())
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