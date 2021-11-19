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
		const Player* m_trackingPlayer = nullptr;	//�ǔ�����v���C���[
		Vector3 m_moveDirection = Vector3::Zero;	//�ړ�����
		int m_moveTime = 0;							//�ړ�����
	};
}

