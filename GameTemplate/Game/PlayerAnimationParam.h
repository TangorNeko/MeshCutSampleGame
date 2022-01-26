#pragma once

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
		//プレイヤーの状態
		enum PlayerState
		{
			enIdle,				//待機中
			enWalk,				//歩行中
			enRunning,			//走り中
			enGuard,			//ガード中
			enCutMode,			//切断モード中
			enKnockDown,		//ノックダウン中
			enJumping,			//ジャンプ中
			enUnequip,			//納刀中
			enBackHandSpring,	//後転中
		};

		PlayerState playerState = enIdle;	//現在のプレイヤーの状態
		bool isPlayingAnimation = false;	//アニメーションを再生中?
		int downTime = 0;
		int comboNum = 0;
		int attackingTime = 0;
		int handspringTime = 0;
	};
}