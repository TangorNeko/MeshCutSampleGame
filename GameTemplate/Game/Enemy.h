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
		
		void SetRotation(const Quaternion rot)
		{
			m_qRot = rot;
		}

		void Damage(float damage);
	private:
		SkinModelRender* m_enemyRender = nullptr;
		Vector3 m_position = Vector3::Zero;
		Quaternion m_qRot = Quaternion::Identity;
		float m_hp = 25.0f;
		FontRender* m_canCutTextRender = nullptr;
	};
}