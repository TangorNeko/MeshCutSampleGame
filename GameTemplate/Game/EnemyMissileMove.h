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
		const Player* m_trackingPlayer = nullptr;	//’Ç”ö‚·‚éƒvƒŒƒCƒ„[
		Vector3 m_moveDirection = Vector3::Zero;	//ˆÚ“®•ûŒü
		int m_moveTime = 0;							//ˆÚ“®ŠÔ
	};
}

