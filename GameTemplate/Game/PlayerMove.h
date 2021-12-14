#pragma once
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerMove
	{
	public:

		void Init(Vector3& playerPosition);

		void Move(Vector3& playerPosition,PlayerAnimationParam& animParam);

		void CalcToModelDirectionQRot();

		const Vector3& GetPlayerDirection()
		{
			return m_playerDirection;
		}

		void SetPlayerDirection(const Vector3& direction)
		{
			m_playerDirection = direction;
		}

		CharacterController& GetCharaCon()
		{
			return m_charaCon;
		}

		const Quaternion& GetPlayerDirectionRot()
		{
			return m_toMoveDirectionRot;
		}

		void KnockDown(const Vector3& moveAmount);
	private:
		CharacterController m_charaCon;							//プレイヤーのキャラコン
		Vector3 m_moveAmount = Vector3::Zero;					//プレイヤーの移動量
		Vector3 m_prevPosition = Vector3::Zero;					//1フレーム前のプレイヤーの座標
		Vector3 m_playerDirection = Vector3::Front;				//プレイヤーの向き
		Quaternion m_toMoveDirectionRot = Quaternion::Identity;	//プレイヤーの移動方向へのクォータニオン
		int m_aerialFrame = 0;									//空中にいるフレーム数

		//TODO:変数の必要性を検証したのち正式にm_をつけメンバー変数とする
		bool isKnockDown = false;
		int knockDownFrame = 0;
		Vector3 knockDownAmount = Vector3::Zero;

		//TODO:仮ミサイルジャンプ用変数
		Vector3 m_targetPos[4] = { Vector3::Zero,Vector3::Zero,Vector3::Zero ,Vector3::Zero };
		int m_moveState = 0;
		int m_targetCount = 0;
		int m_jumpFrameCount = 0;
		int m_distanceCount = 0;
		bool m_isMovingUp = true;
		bool m_isMoveStartFrame = true;
	};
}

