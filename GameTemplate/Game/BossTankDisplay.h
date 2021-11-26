#pragma once

namespace Game
{
	class BossTankDisplay
	{
	public:
		~BossTankDisplay();

		void Init(int hp);

		void Update(int hp,const Vector3& bossPosition);

	private:
		FontRender* m_hpRender = nullptr;
		FontRender* m_cutStateRender = nullptr;
	};
}