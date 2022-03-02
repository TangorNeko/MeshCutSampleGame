#include "stdafx.h"
#include "Phase4.h"
#include "BossTank.h"
#include "GameScene.h"

namespace
{
	int PHASE_START_FRAME = 180;									//�t�F�[�Y���J�n�����܂ł̃t���[����
	const Vector3 BOSS_POSITION = { 0.0f,0.0f,-1300.0f };			//�{�X������������W
	const wchar_t* WARP_SOUND_PATH = L"Assets/sound/WarpSE.wav";	//�{�X�o�����ʉ��p�X
}

namespace Game
{
	void Phase4::Init()
	{
		//�t�F�[�Y�J�n�܂ł̃t���[�����Z�b�g
		SetPhaseStartFrame(PHASE_START_FRAME);
	}

	void Phase4::PhaseStart()
	{
		//�{�X������
		BossTank* bossTank = NewGO<BossTank>(Priority::High, "bosstank");
		bossTank->SetPosition(BOSS_POSITION);

		//�����̌��ʉ����Đ�
		SoundOneShotPlay(WARP_SOUND_PATH);

		//BGM��ύX����
		GameScene* gameScene = FindGO<GameScene>("gamescene");
		gameScene->GameBGMChange();
	}

	bool Phase4::PhaseUpdate()
	{
		//�{�X�����邩������
		BossTank* bosstank = FindGO<BossTank>("bosstank");

		//�{�X�����Ȃ�������
		if (bosstank == nullptr)
		{
			//�t�F�[�Y�I��
			return true;
		}

		return false;
	}
}
