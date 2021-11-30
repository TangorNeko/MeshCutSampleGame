#pragma once

namespace Game
{
	class BackGround;
	class Player;
	class EnemyMissile;
	class BossTank;

	class GameScene : public IGameObject
	{
		~GameScene();

		bool Start() override;

		void Update() override;

	private:
		BackGround* m_backGround = nullptr;						//�X�e�[�W�N���X
		Player* m_player = nullptr;								//�v���C���[�N���X
		BossTank* m_bossTank = nullptr;							//��Ԃ̃{�X�N���X
	};
}

