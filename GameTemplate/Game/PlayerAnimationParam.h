#pragma once
#include "stdafx.h"

namespace Game
{

	//プレイヤーのアニメーション一覧
	enum PlayerAnimationEnum
	{
		enAnim_Idle,//待機
		enAnim_Walk,//歩行
		enAnim_Attack,//攻撃
		enAnim_Attack2,
		enAnim_Attack3,
		enAnim_Num//アニメーションの数
	};

	//プレイヤーのアニメーション遷移に使うパラメーター
	struct PlayerAnimationParam
	{
		bool isWalking = false;
		int attackNum = false;
		float attackingTime = 0.0f;
		Vector3 playerPosition = Vector3::Zero;
	};
}