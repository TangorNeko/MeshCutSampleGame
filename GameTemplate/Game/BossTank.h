#pragma once
#include "EnemyTask.h"
#include "BossTankCollision.h"
#include "BossTankTasks.h"
#include "BossTankDisplay.h"

namespace Game
{
	class BossTank : public IGameObject
	{
	public:
		~BossTank();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce) override;

		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		Vector3 GetPosition()
		{
			return m_position;
		}

		void Damage(float damage);

		void SetTask(int taskNum, EnemyTask task)
		{
			m_tankTask[taskNum] = task;
		}

		void SetTurretDeg(float deg)
		{
			m_turretDeg = deg;
		}

		float GetTurretDeg()
		{
			return m_turretDeg;
		}
		
		void SetBaseDeg(float deg)
		{
			m_baseDeg = deg;
		}

		float GetBaseDeg()
		{
			return m_baseDeg;
		}
	private:
		Vector3 m_position = Vector3::Zero;
		BossTankCollision m_bossTankCollision;
		SkinModelRender* m_baseRender = nullptr;
		SkinModelRender* m_turretRender = nullptr;
		SkinModelRender* m_cannonRender = nullptr;
		BossTankDisplay m_bossTankDisplay;
		Quaternion m_baseRot = Quaternion::Identity;
		Quaternion m_turretRot = Quaternion::Identity;
		float m_baseDeg = 0.0f;
		float m_turretDeg = 0.0f;
		int m_hp = 1000;
		bool m_isCannonBreak = false;
		bool m_isTurretBreak = false;
		bool m_isBaseBreak = false;
		EnemyTask m_tankTask[BossTankTasks::enTaskNum];
		std::queue<EnemyTask> m_taskQueue;
		bool m_isSummonMinions = false;
	};
}

