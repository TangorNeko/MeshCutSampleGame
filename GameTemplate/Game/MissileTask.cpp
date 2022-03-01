#include "stdafx.h"
#include "MissileTask.h"
#include "BossTank.h"
#include "EnemyMissile.h"

namespace
{
	const int MISSILE_TIME_FIRST = 40;											//1���ڂ̃~�T�C�����˃t���[��
	const int MISSILE_TIME_SECOND = 70;											//2���ڂ̃~�T�C�����˃t���[��
	const int MISSILE_TIME_THIRD = 100;											//3���ڂ̃~�T�C�����˃t���[��
	const wchar_t* MISSILE_SOUND_PATH = L"Assets/sound/MissileLaunchSE.wav";	//�~�T�C�����˂̌��ʉ��p�X
	const char16_t* EFFECT_WARNING_PATH = u"Assets/effect/Warning.efk";			//�x���G�t�F�N�g�p�X
	const Vector3 EFFECT_WARNING_HEIGHT = { 0.0f,250.0f,0.0f };					//�x���G�t�F�N�g�̍���
}

namespace Game
{
	void MissileTask::SubmitTo(BossTank* bossTank,int taskNum)
	{
		EnemyTask MissileTask;

		//�~�T�C���𔭎˂��鏈��
		auto ShotMissile = [bossTank](const Vector3& position)
		{
			//�~�T�C�����쐬
			EnemyMissile* missile = NewGO<EnemyMissile>(Priority::High, "missile");
			missile->SetPosition(position);

			//���˃T�E���h���Đ�
			SoundOneShotPlay(MISSILE_SOUND_PATH);
		};

		MissileTask.SetStartFunc([bossTank]()
			{
				//�x���G�t�F�N�g���Đ�
				Effect* warningEffect = NewGO<Effect>(Priority::High);
				warningEffect->Init(EFFECT_WARNING_PATH);
				warningEffect->SetPosition(bossTank->GetPosition() + EFFECT_WARNING_HEIGHT);
				warningEffect->Play();
			}
		);

		MissileTask.SetUpdateFunc([bossTank, ShotMissile](int taskTime)
			{
				//�K��t���[���ɂȂ�ƃ~�T�C���𔭎˂���
				if (taskTime == MISSILE_TIME_FIRST)
				{
					ShotMissile(bossTank->GetRightCannonPosition());
				}

				if (taskTime == MISSILE_TIME_SECOND)
				{
					ShotMissile(bossTank->GetLeftCannonPosition());
				}

				if (taskTime == MISSILE_TIME_THIRD)
				{
					ShotMissile(bossTank->GetRightCannonPosition());

					//�O���ڂ���������^�X�N�͏I��
					return true;
				}

				return false;
			}
		);

		//�{�X�ɂ��̃^�X�N���Z�b�g����
		bossTank->SetTask(taskNum, MissileTask);
	}
}