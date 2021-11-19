#pragma once

namespace Game
{
	class Player;

	class EnemyMissileMove
	{
	public:
		void FindTarget();

		void Update(Vector3& position);
	private:
		const Player* m_trackingPlayer = nullptr;	//追尾するプレイヤー
		Vector3 m_moveDirection = Vector3::Zero;	//移動方向
		int m_moveTime = 0;							//移動時間
	};
}

