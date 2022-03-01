#include "stdafx.h"
#include "RollingTask.h"
#include "BossTank.h"
#include "Player.h"

namespace
{
	const float ROLLING_DEG = 15.0f;									//1�t���[���̉�]�p�x
	const int ROLLING_TIME_START = 40;									//��]�U���̊J�n�t���[��
	const int ROLLING_TIME_END = 63;									//��]�U���̏I���t���[��
	const float ROLLING_RANGE = 700.0f;									//������΂��͈�
	const float ROLLING_KNOCKDOWN_POWER = 60.0f;						//������΂���
	const wchar_t* ROLLING_SOUND_PATH = L"Assets/sound/RollingSE.wav";	//��]�̌��ʉ��p�X
	const wchar_t* DAMAGE_SOUND_PATH = L"Assets/sound/DamageSE.wav";	//�_���[�W�̌��ʉ��p�X
	const char16_t* EFFECT_WARNING_PATH = u"Assets/effect/Warning.efk";	//�x���G�t�F�N�g�p�X
	const Vector3 EFFECT_WARNING_HEIGHT = { 0.0f,250.0f,0.0f };			//�x���G�t�F�N�g�̍���
}

namespace Game
{
	void RollingTask::SubmitTo(BossTank* bossTank, int taskNum)
	{
		EnemyTask RollingTask;

		RollingTask.SetStartFunc([bossTank]()
			{
				//�x���G�t�F�N�g���Đ�
				Effect* warningEffect = NewGO<Effect>(Priority::High);
				warningEffect->Init(EFFECT_WARNING_PATH);
				warningEffect->SetPosition(bossTank->GetPosition() + EFFECT_WARNING_HEIGHT);
				warningEffect->Play();
			}
		);

		RollingTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				if (taskTime == ROLLING_TIME_START)
				{
					//��]�T�E���h���Đ�
					SoundOneShotPlay(ROLLING_SOUND_PATH);
				}

				if (taskTime >= ROLLING_TIME_START)
				{
					//���t���[����������]�����Ă���
					bossTank->SetTurretDeg(bossTank->GetTurretDeg() + ROLLING_DEG);
				}

				//�K��t���[���ɂȂ�����I��
				if (taskTime == ROLLING_TIME_END)
				{
					//�v���C���[�Ƃ̋������擾
					Player* player = FindGO<Player>("player");
					Vector3 distance = player->GetPosition() - bossTank->GetPosition();

					//�v���C���[�Ƃ̋������߂�������
					if (distance.LengthSq() < ROLLING_RANGE * ROLLING_RANGE)
					{
						distance.y = 0.0f;
						distance.Normalize();

						//�v���C���[��傫��������΂�
						player->KnockDown(distance * ROLLING_KNOCKDOWN_POWER);
						SoundOneShotPlay(DAMAGE_SOUND_PATH);
					}

					return true;
				}

				return false;
			}
		);

		//�{�X�ɂ��̃^�X�N���Z�b�g����
		bossTank->SetTask(taskNum, RollingTask);
	}
}