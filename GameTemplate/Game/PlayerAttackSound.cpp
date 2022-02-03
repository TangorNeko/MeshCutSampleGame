#include "stdafx.h"
#include "PlayerAttackSound.h"

namespace
{
	const int MISSSOUND_NUM = 3;
	const wchar_t* MISSSOUND_PATH[MISSSOUND_NUM] =
	{
		L"Assets/sound/MissSE1.wav",
		L"Assets/sound/MissSE2.wav",
		L"Assets/sound/MissSE3.wav"
	};

	const int HITSOUND_NUM = 3;
	const wchar_t* HITSOUND_PATH[HITSOUND_NUM] =
	{
		L"Assets/sound/HitSE1.wav",
		L"Assets/sound/HitSE2.wav",
		L"Assets/sound/HitSE3.wav"
	};
}

namespace Game
{
	void PlayerAttackSound::PlayMissSound(int comboNum)
	{
		//�R���{�̒i���ɍ��킹�ă~�X�̉����Đ�
		SoundOneShotPlay(MISSSOUND_PATH[comboNum]);
	}
	
	void PlayerAttackSound::PlayHitSound(int comboNum)
	{
		//�R���{�̒i���ɍ��킹�ăq�b�g�����Đ�
		SoundOneShotPlay(HITSOUND_PATH[comboNum]);
	}
}
