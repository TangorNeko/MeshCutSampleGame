#pragma once

namespace Game
{
	class BossTank : public IGameObject
	{
	public:
		~BossTank();

		bool Start() override;

		void Update() override;

		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
	private:
		Vector3 m_position = Vector3::Zero;
		SkinModelRender* m_baseRender;
		SkinModelRender* m_turretRender;
		SkinModelRender* m_cannonRender;
		Quaternion m_baseRot;
		Quaternion m_turretRot;
		float m_baseDeg;
		float m_turretDeg;
		float m_hp = 100.0f;
	};
}

