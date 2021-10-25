#pragma once

namespace Game
{
	class BackGround;
	class Player;
	class Enemy;

	class GameScene : public IGameObject
	{
		~GameScene();

		bool Start() override;

		void Update() override;

	private:
		BackGround* m_backGround = nullptr;
		Player* m_player = nullptr;
		Enemy* m_enemy = nullptr;
	};
}

