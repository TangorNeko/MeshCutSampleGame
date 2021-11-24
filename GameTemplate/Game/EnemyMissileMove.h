#pragma once

namespace Game
{
	class Player;

	class EnemyMissileMove
	{
	public:
		EnemyMissileMove();

		void SetTarget(const Player* targetPlayer)
		{
			m_trackingPlayer = targetPlayer;
		}

		void Update(Vector3& position);

		const Vector3& GetDirection()
		{
			return m_moveDirection;
		}

		

	private:
		
		void CalcAvg(Vector3& direction);
	private:
		const Player* m_trackingPlayer = nullptr;	//�ǔ�����v���C���[
		Vector3 m_moveDirection = Vector3::Zero;	//�ړ�����
		std::list<Vector3> m_pastDirectionList;
		int m_moveTime = 0;							//�ړ�����
	};
}

