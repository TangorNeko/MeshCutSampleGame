#pragma once

namespace Game
{
	class AttackTutorial : public IGameObject
	{
		~AttackTutorial();

		bool Start() override;

		void Update() override;

	private:
		SpriteRender* m_spriteRender = nullptr;
		int m_frame = 0;
	};
}

