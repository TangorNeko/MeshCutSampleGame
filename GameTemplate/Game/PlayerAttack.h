#pragma once

namespace Game
{
	class PlayerAttack
	{
	public:
		~PlayerAttack();

		void Update(const Vector3& playerPosition);

		SkinModelRender* GetModel()
		{
			return m_testHitBox;
		}

	private:
		SkinModelRender* m_testHitBox = nullptr;
		float m_attackTime = 0.0f;				//UŒ‚ŠÔ
		bool m_isAttacking = false;				//UŒ‚’†?
	};
}

