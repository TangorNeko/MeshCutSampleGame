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
		BackGround* m_backGround = nullptr;						//�X�e�[�W�N���X
		Player* m_player = nullptr;								//�v���C���[�N���X
		Enemy* m_enemy[4] = { nullptr,nullptr,nullptr,nullptr };//�G�l�~�[�N���X
	};
}

