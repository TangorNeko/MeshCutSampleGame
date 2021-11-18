#pragma once

namespace Game
{
	class BossTank : public IGameObject
	{
	public:
		~BossTank();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* skinModelRender) override;

		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		Vector3 GetPosition()
		{
			return m_position;
		}

		void Damage(float damage);
	private:
		Vector3 m_position = Vector3::Zero;
		BoxCollider m_boxCollider;
		RigidBody m_rigidBody;
		SkinModelRender* m_baseRender = nullptr;
		SkinModelRender* m_turretRender = nullptr;
		SkinModelRender* m_cannonRender = nullptr;
		FontRender* m_fontRender = nullptr;
		FontRender* m_hpRender = nullptr;
		Quaternion m_baseRot;
		Quaternion m_turretRot;
		float m_baseDeg;
		float m_turretDeg;
		int m_hp = 1000;
		bool m_isCannonBreak = false;
	};
}

