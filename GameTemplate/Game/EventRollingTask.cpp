#include "stdafx.h"
#include "EventRollingTask.h"
#include "BossTank.h"
#include "Player.h"

namespace
{
	const float ROLLING_DEG = 15.0f;									//1�t���[���̉�]�p�x
	const int ROLLING_TIME_START = 40;									//��]�U���̊J�n�t���[��
	const int ROLLING_TIME_END = 63;									//��]�U���̏I���t���[��
	const float ROLLING_KNOCKDOWN_POWER = 60.0f;						//������΂���
	const wchar_t* ROLLING_SOUND_PATH = L"Assets/sound/RollingSE.wav";	//��]�̌��ʉ��p�X
}

namespace Game
{
	void EventRollingTask::SubmitTo(BossTank* bossTank, int taskNum)
	{
		EnemyTask EventRollingTask;

		EventRollingTask.SetUpdateFunc([bossTank](int taskTime)->bool
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
					//�v���C���[�ւ̌������擾
					Player* player = FindGO<Player>("player");
					Vector3 distance = player->GetPosition() - bossTank->GetPosition();

					distance.y = 0.0f;
					distance.Normalize();

					//�v���C���[��傫��������΂�
					player->KnockDown(distance * ROLLING_KNOCKDOWN_POWER);

					return true;
				}

				return false;
			}
		);

		//�{�X�ɂ��̃^�X�N���Z�b�g����
		bossTank->SetTask(taskNum, EventRollingTask);
	}
}