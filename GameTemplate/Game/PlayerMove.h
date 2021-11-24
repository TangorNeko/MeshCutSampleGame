#pragma once
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerMove
	{
	public:

		void Init(Vector3& playerPosition);

		void Move(Vector3& playerPosition,PlayerAnimationParam& animParam);

		Quaternion CalcToModelDirectionQRot();

		void TurnModelToPlayerDirection(SkinModelRender* modelRender);

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
	private:
		CharacterController m_charaCon;							//プレイヤーのキャラコン
		Vector3 m_moveAmount = Vector3::Zero;					//プレイヤーの移動量
		Vector3 m_prevPosition = Vector3::Zero;					//1フレーム前のプレイヤーの座標
		Vector3 m_playerDirection = Vector3::Front;				//プレイヤーの向き
		Quaternion m_toMoveDirectionRot = Quaternion::Identity;	//プレイヤーの移動方向へのクォータニオン
	};
}

