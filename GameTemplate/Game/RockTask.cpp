#include "stdafx.h"
#include "RockTask.h"
#include "BossTank.h"
#include "EnemyRock.h"
#include "Player.h"

namespace
{
	const float ROCK_SHOT_HEIGHT = 200.0f;								//��𔭎˂��鍂��
	const float ROCK_SHOT_XOFFSET = 100.0f;								//��𔭎˂���X���W�̃I�t�Z�b�g
	const int ROCK_TIME_FIRST = 40;										//1���ڂ̊�𔭎˂���t���[��
	const int ROCK_TIME_SECOND = 45;									//2���ڂ̊�𔭎˂���t���[��
	const int ROCK_TIME_THIRD = 48;										//3���ڂ̊�𔭎˂���t���[��
	const char16_t* EFFECT_WARNING_PATH = u"Assets/effect/Warning.efk";	//�x���G�t�F�N�g�p�X
	const Vector3 EFFECT_WARNING_HEIGHT = { 0.0f,250.0f,0.0f };			//�x���G�t�F�N�g�̍���
}

namespace Game
{
	void RockTask::SubmitTo(BossTank* bossTank, int taskNum)
	{
		EnemyTask RockTask;

		RockTask.SetStartFunc([bossTank]()
			{
				//�x���G�t�F�N�g���Đ�
				Effect* warningEffect = NewGO<Effect>(Priority::High);
				warningEffect->Init(EFFECT_WARNING_PATH);
				warningEffect->SetPosition(bossTank->GetPosition() + EFFECT_WARNING_HEIGHT);
				warningEffect->Play();
			}
		);

		//����쐬����֐�
		auto ShotRock = [bossTank](const Vector3& pos) {
			EnemyRock* rock = NewGO<EnemyRock>(Priority::High, "rock");
			Player* player = FindGO<Player>("player");
			Vector3 distance = player->GetPosition() - bossTank->GetPosition();
			distance.Normalize();
			rock->SetPosition(pos);
			rock->SetDirection(distance);
		};

		RockTask.SetUpdateFunc([bossTank, ShotRock](int taskTime)
			{
				//�K��t���[���ɂȂ�Ɗ�𔭎�
				if (taskTime == ROCK_TIME_FIRST)
				{
					Vector3 pos = bossTank->GetPosition();
					pos.y += ROCK_SHOT_HEIGHT;
					ShotRock(pos);
				}

				if (taskTime == ROCK_TIME_SECOND)
				{
					Vector3 pos = bossTank->GetPosition();
					pos.y += ROCK_SHOT_HEIGHT;
					pos.x += ROCK_SHOT_XOFFSET;
					ShotRock(pos);
				}

				if (taskTime == ROCK_TIME_THIRD)
				{
					Vector3 pos = bossTank->GetPosition();
					pos.y += ROCK_SHOT_HEIGHT;
					pos.x -= ROCK_SHOT_XOFFSET;
					ShotRock(pos);

					//3���˂�����^�X�N�͏I��
					return true;
				}


				return false;
			}
		);

		//�{�X�ɂ��̃^�X�N���Z�b�g����
		bossTank->SetTask(taskNum, RockTask);
	}
}