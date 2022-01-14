#pragma once

namespace Game
{
	class DashTutorial : public IGameObject
	{
		~DashTutorial();

		bool Start() override;

		void Update() override;

	private:
		SpriteRender* m_spriteRender = nullptr;
		int m_frame = 0;
	};
}