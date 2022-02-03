#pragma once

namespace Game
{
	class BackGround;
	class Player;
	class EnemyMissile;
	class BossTank;
	class GameSceneBGM;

	class GameScene : public IGameObject
	{
	public:
		~GameScene();

		bool Start() override;

		void Update() override;

		/**
		 * @brief �Q�[���V�[���Ő������ꂽ�������폜����
		*/
		void DeleteProducts();

		/**
		 * @brief �Q�[���I�[�o�[��ʒm����
		*/
		void NotifyGameOver();
	private:
		BackGround* m_backGround = nullptr;						//�X�e�[�W�N���X
		Player* m_player = nullptr;								//�v���C���[�N���X
		BossTank* m_bossTank = nullptr;							//��Ԃ̃{�X�N���X
		GameSceneBGM* m_gameSceneBGM = nullptr;					//BGM�Đ��N���X
		int m_phase = 0;										//�Q�[���V�[���̃t�F�[�Y
		int m_phaseWaitTime = 0;								//�t�F�[�Y�̑ҋ@����
		bool m_isGameOver = false;								//�Q�[���I�[�o�[�ɂȂ���?
	};
}

