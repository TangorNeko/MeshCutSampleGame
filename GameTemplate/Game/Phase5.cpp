#include "stdafx.h"
#include "Phase5.h"
#include "Fade.h"
#include "GameScene.h"


namespace
{
	const int FADE_START_TIME = 280;		//�t�F�[�h���J�n�����t���[��
	const int GAMESCENE_DELETE_TIME = 380;	//�Q�[���V�[�����폜���čĐ�������t���[��
	const float FADE_RATE = 0.01f;			//�t�F�[�h��
	const int FADE_WAIT_FRAME = 10;			//�t�F�[�h������������ҋ@����t���[����
}

namespace Game
{
	bool Phase5::PhaseUpdate()
	{
		//�t�F�[�h���J�n�����t���[����������
		if (GetPhaseWaitFrame() == FADE_START_TIME)
		{
			//�t�F�[�h���J�n����
			StartFade();
		}

		//�Q�[���V�[�����폜���čĐ�������t���[����������
		if (GetPhaseWaitFrame() == GAMESCENE_DELETE_TIME)
		{
			//���݂̃Q�[���V�[�����폜
			DeleteGO(FindGO<GameScene>("gamescene"));

			//�V�����Q�[���V�[���𐶐�
			NewGO<GameScene>(Priority::High, "gamescene");
		}

		return false;
	}

	void Phase5::StartFade()
	{
		//�t�F�[�h���J�n������
		Fade* fade = NewGO<Fade>(Priority::High);
		fade->SetFadeInRate(FADE_RATE);
		fade->SetWaitFrame(FADE_WAIT_FRAME);
		fade->SetFadeOutRate(FADE_RATE);
	}
}
