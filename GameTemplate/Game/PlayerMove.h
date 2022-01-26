#pragma once
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerMove
	{
	public:

		void Init(Vector3& playerPosition);

		bool Move(Vector3& playerPosition,PlayerAnimationParam& animParam);

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

		void BackHandSpring(const Vector3& moveAmount);

		void NoticeMissileMoveStart()
		{
			m_playerMoveEvent = enMissileMove;
		}

		void NoticeFrontMoveStart()
		{
			m_playerMoveEvent = enFrontMove;

			//ボスの方に向かせる
			SetPlayerDirection(Vector3::Back);
		}

		void NoticeMissileMoveEnd()
		{
			m_isMissileMove = false;

			m_playerMoveEvent = enNormal;
		}

		void NoticeFrontMoveEnd()
		{
			m_isFrontMove = false;

			m_playerMoveEvent = enNormal;
		}

		void DisableMove()
		{
			m_playerMoveEvent = enDead;
		}

		bool NormalMove(Vector3& playerPosition, PlayerAnimationParam& animParam);

		bool MissileMove(Vector3& playerPosition, PlayerAnimationParam& animParam);

		bool FrontMove(Vector3& playerPosition, PlayerAnimationParam& animParam);

		bool BackHandspringMove(Vector3& playerPosition, PlayerAnimationParam& animParam);

		bool KnockDownMove(Vector3& playerPosition, PlayerAnimationParam& animParam);

		bool DeadMove()
		{
			return false;
		}

		void MissileMoveSuccess()
		{
			m_isMissileMove = true;
		}

	private:
		CharacterController m_charaCon;							//プレイヤーのキャラコン
		Vector3 m_moveAmount = Vector3::Zero;					//プレイヤーの移動量
		Vector3 m_prevPosition = Vector3::Zero;					//1フレーム前のプレイヤーの座標
		Vector3 m_playerDirection = Vector3::Front;				//プレイヤーの向き
		Quaternion m_toMoveDirectionRot = Quaternion::Identity;	//プレイヤーの移動方向へのクォータニオン
		int m_aerialFrame = 0;									//空中にいるフレーム数

		//TODO:変数の必要性を検証したのち正式にm_をつけメンバー変数とする
		int knockDownFrame = 0;
		Vector3 knockDownAmount = Vector3::Zero;

		//TODO:仮ミサイルジャンプ用変数
		bool m_isMissileMove = false;
		Vector3 m_targetPos[5] = { Vector3::Zero,Vector3::Zero,Vector3::Zero ,Vector3::Zero,Vector3::Zero };
		int m_moveState = 0;
		int m_targetCount = 0;
		int m_jumpFrameCount = 0;
		int m_distanceCount = 0;
		bool m_isMovingUp = true;
		bool m_isMoveStartFrame = true;

		//TODO:ボス前ジャンプ用変数
		bool m_isFrontMove = false;
		bool m_isJumpStartFrame = true;
		Vector3 m_frontMoveAmount = Vector3::Zero;

		//TODO:後転用変数
		int backHandspringFrame = 0;
		Vector3 backHandSpringAmount = Vector3::Zero;

		enum MoveEvent
		{
			enNormal,
			enMissileMove,
			enFrontMove,
			enBackHandspring,
			enKnockDown,
			enDead,
		};

		MoveEvent m_playerMoveEvent = enNormal;
	};
}

