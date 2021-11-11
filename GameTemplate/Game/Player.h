#pragma once
#include "PlayerMove.h"
#include "PlayerCamera.h"
#include "PlayerAnimation.h"
#include "PlayerAttack.h"
#include "PlayerCut.h"

namespace Game
{
	class Player : public IGameObject
	{
		~Player();

		bool Start() override;

		void Update() override;

	private:
		SkinModelRender* m_playerModel = nullptr;	//プレイヤーのモデル
		Vector3 m_position = Vector3::Zero;			//プレイヤーの座標
		PlayerMove m_playerMove;					//プレイヤーの移動クラス
		PlayerCamera m_playerCamera;				//プレイヤーのカメラクラス
		PlayerAnimation m_playerAnimation;			//プレイヤーのアニメーションクラス
		PlayerAnimationParam m_playerAnimationParam;//プレイヤーのアニメーション制御ステータス
		PlayerAttack m_playerAttack;				//プレイヤーの攻撃クラス
		PlayerCut m_playerCut;						//プレイヤーの切断クラス
	};
}

