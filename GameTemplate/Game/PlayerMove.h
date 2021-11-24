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

		void TurnModelToMoveDirection(SkinModelRender* modelRender);

		CharacterController& GetCharaCon()
		{
			return m_charaCon;
		}
	private:
		CharacterController m_charaCon;							//プレイヤーのキャラコン
		Vector3 m_moveAmount = Vector3::Zero;					//プレイヤーの移動量
		Vector3 m_prevPosition = Vector3::Zero;					//1フレーム前のプレイヤーの座標
		Quaternion m_toMoveDirectionRot = Quaternion::Identity;	//プレイヤーの移動方向へのクォータニオン
	};
}

