#include "stdafx.h"
#include "Phase2.h"
#include "MiniEnemy.h"

namespace
{
	int PHASE_START_FRAME = 180;								//�t�F�[�Y���J�n�����܂ł̃t���[����
	const int SUMMON_NUM_PHASE2 = 2;							//�G���G�̏�����
	const Vector3 MINION_POSITIONS_PHASE2[SUMMON_NUM_PHASE2] =	//�G���G������������W
	{
		{-400.0f,0.0f,-800.0f},
		{400.0f,0.0f,-800.0f}
	};
	const wchar_t* WARP_SOUND_PATH = L"Assets/sound/WarpSE.wav";//�G���G�o�����ʉ��p�X
}

namespace Game
{
	void Phase2::Init()
	{
		//�t�F�[�Y�J�n�܂ł̃t���[�����Z�b�g
		SetPhaseStartFrame(PHASE_START_FRAME);
	}

	void Phase2::PhaseStart()
	{
		//�G���G�𕡐�����
		MiniEnemy* enemy[SUMMON_NUM_PHASE2];
		for (int i = 0; i < SUMMON_NUM_PHASE2; i++)
		{
			enemy[i] = NewGO<MiniEnemy>(Priority::High, "enemy");
			enemy[i]->SetPosition(MINION_POSITIONS_PHASE2[i]);
		}

		//�����̌��ʉ����Đ�
		SoundOneShotPlay(WARP_SOUND_PATH);
	}

	bool Phase2::PhaseUpdate()
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