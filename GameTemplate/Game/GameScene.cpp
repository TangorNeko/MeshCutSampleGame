#include "stdafx.h"
#include "GameScene.h"
#include "BackGround.h"
#include "Player.h"
#include "ProductsDeleter.h"
#include "GameSceneBGM.h"

namespace
{
	const Vector3 PLAYER_START_POSITION = { -1.0f,30.0f,3700.0f };	//�v���C���[�̃X�^�[�g���W
	const int GAMEOVER_PHASE = 5;									//�Q�[���I�[�o�[�̃t�F�[�Y�ԍ�
}

namespace Game
{
	GameScene::~GameScene()
	{
		//�e��I�u�W�F�N�g���폜
		DeleteGO(m_backGround);
		DeleteGO(m_player);
		DeleteGO(m_gameSceneBGM);
		DeleteProducts();
	}

	bool GameScene::Start()
	{
		//�Q�[���̔w�i���쐬
		m_backGround = NewGO<BackGround>(Priority::High,"background");

		//�v���C���[���쐬
		m_player = NewGO<Player>(Priority::High, "player");
		m_player->SetPosition(PLAYER_START_POSITION);
		m_player->EventCutOn();

		//BGM���쐬
		m_gameSceneBGM = NewGO<GameSceneBGM>(Priority::High, "gamescenebgm");

		//�Q�[���t�F�[�Y�̏�����
		m_gamephase.Init();
		return true;
	}

	void GameScene::Update()
	{
		//�t�F�[�Y�̃A�b�v�f�[�g����
		m_gamephase.Update();
	}

	void GameScene::NotifyGameOver()
	{
		//�Q�[���t�F�[�Y���Q�[���I���̂��̂�
		m_gamephase.SkipTo(GAMEOVER_PHASE);
	}

	void GameScene::DeleteProducts()
	{
		//�Q�[���V�[���ɂ���Đ������ꂽ�I�u�W�F�N�g���폜����
		ProductsDeleter productsDeleter;
		productsDeleter.DeleteProducts();
	}

	void GameScene::GameBGMChange()
	{
		//�Q�[��BGM����i�K�i�s������
		m_gameSceneBGM->SoundChange();
	}
}