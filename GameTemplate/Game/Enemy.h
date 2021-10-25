#pragma once

namespace Game
{
	class Enemy : public IGameObject
	{
		~Enemy();

		bool Start() override;

		void Update() override;

	public:
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		const Vector3& GetPosition()
		{
			return m_position;
		}

		void Damage(float damage);
	private:
		SkinModelRender* m_enemyRender = nullptr;
		Vector3 m_position = Vector3::Zero;
		float m_hp = 100.0f;
	};
}