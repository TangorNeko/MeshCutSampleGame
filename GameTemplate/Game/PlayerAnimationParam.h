#pragma once
#include "../../MiniEngine/math/Vector.h"

namespace Game
{

	//プレイヤーのアニメーション一覧
	enum PlayerAnimationEnum
	{
		enAnim_Idle,//待機
		enAnim_Walk,//歩行
		enAnim_Run,//走り
		enAnim_Attack,//コンボ1
		enAnim_Attack2,//コンボ2
		enAnim_Attack3,//コンボ3
		enAnim_Guard,//ガード
		enAnim_CutMode,//切断モード
		enAnim_KnockDown,//ノックダウン
		enAnim_GetUp,//起き上がる
		enAnim_Jump,//ジャンプ
		enAnim_Unequip,//納刀
		enAnim_Idle_Unequip,//納刀状態での待機
		enAnim_BackHandspring,//後転
		enAnim_Num//アニメーションの数
	};

	//プレイヤーのアニメーション遷移に使うパラメーター
	struct PlayerAnimationParam
	{
		//TODO:今の所すべてのbool値は重複してtrueになることがないので、一つにまとめたい
		bool isWalking = false;
		bool isRunning = false;
		bool isGuarding = false;
		bool isCutMode = false;
		bool isKnockDown = false;
		bool isJumping = false;
		bool isUnequip = false;
		bool isBackHandSpring = false;
		int downTime = 0;
		int comboNum = 0;
		int attackingTime = 0;
		int handspringTime = 0;
		Vector3 playerPosition = Vector3::Zero;
		bool isPlayingAnimation = false;
	};
}