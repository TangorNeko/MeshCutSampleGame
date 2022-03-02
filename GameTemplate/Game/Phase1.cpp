#include "stdafx.h"
#include "Phase1.h"
#include "MiniEnemy.h"
#include "GameScene.h"

namespace
{
	int PHASE_START_FRAME = 180;									//�t�F�[�Y���J�n�����܂ł̃t���[����
	const Vector3 MINION_POSITION_PHASE1 = { 0.0f,0.0f,-500.0f };	//�G���G������������W
	const wchar_t* WARP_SOUND_PATH = L"Assets/sound/WarpSE.wav";	//�G���G�o�����ʉ��p�X
}

namespace Game
{
	void Phase1::Init()
	{
		//�t�F�[�Y�J�n�܂ł̃t���[�����Z�b�g
		SetPhaseStartFrame(PHASE_START_FRAME);
	}

	void Phase1::PhaseStart()
	{
		//�G���G������
		MiniEnemy* enemy = NewGO<MiniEnemy>(Priority::High, "enemy");
		enemy->SetPosition(MINION_POSITION_PHASE1);

		//�����̌��ʉ����Đ�
		SoundOneShotPlay(WARP_SOUND_PATH);
		
		//BGM��ύX����
		GameScene* gameScene = FindGO<GameScene>("gamescene");
		gameScene->GameBGMChange();
	}

	bool Phase1::PhaseUpdate()
	{
		//�G���G�����邩������
		MiniEnemy* enemy = FindGO<MiniEnemy>("enemy");

		//�G���G�����Ȃ�������
		if (enemy == nullptr)
		{
			//�t�F�[�Y�I��
			return true;
		}

		return false;
	}
}