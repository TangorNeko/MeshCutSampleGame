#pragma once
#include "GamePhase.h"

namespace Game
{
	class BackGround;
	class Player;
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

		/**
		 * @brief �Q�[����BGM��1�i�K�ύX����
		*/
		void GameBGMChange();
	private:
		BackGround* m_backGround = nullptr;						//�X�e�[�W�N���X
		Player* m_player = nullptr;								//�v���C���[�N���X
		GameSceneBGM* m_gameSceneBGM = nullptr;					//BGM�Đ��N���X
		GamePhase m_gamephase;									//�Q�[���t�F�[�Y
	};
}

