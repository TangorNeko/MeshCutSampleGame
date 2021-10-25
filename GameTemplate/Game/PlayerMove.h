#pragma once

namespace Game
{
	class PlayerMove
	{
	public:

		void Init(Vector3& playerPosition);

		void Move(Vector3& playerPosition);

		void TurnModelToMoveDirection(SkinModelRender* modelRender);
	private:
		CharacterController m_charaCon;
		Vector3 m_moveAmount = Vector3::Zero;
	};
}

