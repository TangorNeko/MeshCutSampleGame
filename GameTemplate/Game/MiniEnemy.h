#pragma once

namespace Game
{
	class MiniEnemy : public IGameObject
	{
	public:
		~MiniEnemy();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce) override;

		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
	private:
		SkinModelRender* m_baseRender = nullptr;
		SkinModelRender* m_turretRender = nullptr;
		SkinModelRender* m_cannonRender = nullptr;
		Quaternion rotation = Quaternion::Identity;
		Vector3 m_position = Vector3::Zero;
		Vector3 move = Vector3::Zero;
		int m_frame = 0;
		float m_turretDeg = 0.0f;
		bool m_isDead = false;
	};
}

