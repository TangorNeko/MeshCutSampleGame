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

		for (int i = 0;i<enAnim_Num;i++)
		{
			m_animationState[i] = new AnimationState<PlayerAnimationParam, PlayerAnimations>;
			m_animationState[i]->SetState(static_cast<PlayerAnimations>(i));
		}

		m_animationState[enAnim_Idle]->AddCondition([this](const PlayerAnimationParam& param)->PlayerAnimations
			{
				//前の位置と変わってた場合歩きアニメーション、それ以外は待機アニメーション
				if (param.isAttacking == true)
				{
					return enAnim_Attack;
				}
				
				if (param.isWalking == true)
				{
					return enAnim_Walk;
				}

				return m_animationState[enAnim_Idle]->GetMyState();
			}
		);

		m_animationState[enAnim_Walk]->AddCondition([this](const PlayerAnimationParam& param)->PlayerAnimations
			{
				if (param.isAttacking == true)
				{
					return enAnim_Attack;
				}

				if (param.isWalking == false)
				{
					return enAnim_Idle;
				}

				return m_animationState[enAnim_Walk]->GetMyState();
			}
		);

		m_animationState[enAnim_Attack]->AddCondition([this](const PlayerAnimationParam& param)->PlayerAnimations
			{
				if (param.isAttacking == false)
				{
					return enAnim_Idle;
				}

				return m_animationState[enAnim_Attack]->GetMyState();
			}
		);
			
		//初期状態で再生されるアニメーションのステートを格納
		m_playingAnimState = m_animationState[enAnim_Idle];
	}

	void PlayerAnimation::Update(SkinModelRender* playerRender)
	{

		if (m_prevPosition.LengthSq() != playerRender->GetPosition().LengthSq())
		{
			m_animationParam.isWalking = true;
		}
		else
		{
			m_animationParam.isWalking = false;
		}

		if (g_pad[0]->IsTrigger(enButtonY) && m_animationParam.isAttacking == false)
		{
			m_animationParam.isAttacking = true;
			m_attackingTime = 0.0f;
		}

		if (m_animationParam.isAttacking == true)
		{
			m_attackingTime += 1.0f;

			if (m_attackingTime > 60.0f)
			{
				m_animationParam.isAttacking = false;
			}
		}

		//アニメーションのスピードをセット
		playerRender->SetAnimationSpeed(SPEED_PLAYER_ANIM);

		PlayerAnimations nextAnim = m_playingAnimState->StateChangeCheck(m_animationParam);

		playerRender->PlayAnimation(nextAnim);
		m_playingAnimState = m_animationState[nextAnim];


		//最後にプレイヤーの位置を取得
		m_prevPosition = playerRender->GetPosition();
	}
}