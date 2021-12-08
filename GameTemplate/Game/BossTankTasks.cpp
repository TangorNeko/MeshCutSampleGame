#include "stdafx.h"
#include "BossTankTasks.h"
#include "BossTank.h"
#include "EnemyMissile.h"
#include "Player.h"
#include "MiniEnemy.h"
#include "EnemyRock.h"

namespace
{
	const int MISSILE_TIME_FIRST = 30;
	const int MISSILE_TIME_SECOND = 60;
	const int MISSILE_TIME_THIRD = 90;
	const float MISSILE_SHOT_HEIGHT = 200.0f;
	const float ROLLING_DEG = 15.0f;
	const int ROLLING_TIME_END = 23;
	const float ROLLING_RANGE = 700.0f;
	const float ROLLING_KNOCKDOWN_POWER = 60.0f;
	const int SUMMON_TIME = 30;
	const int SUMMON_NUM = 4;
	const Vector3 MINION_POSITIONS[SUMMON_NUM] = { { 850.0f,0.0f,-1400.0f }, { -850.0f,0.0f,-1400.0f } ,{ 1300.0f,0.0f,-1000.0f } ,{ -1300.0f,0.0f,-1000.0f } };
	const float ROCK_SHOT_HEIGHT = 200.0f;
	const float ROCK_SHOT_XOFFSET = 100.0f;
	const int ROCK_TIME_FIRST = 30;
	const int ROCK_TIME_SECOND = 35;
	const int ROCK_TIME_THIRD = 38;
	const int WAIT_TIME = 180;
}

namespace Game
{
	void BossTankTasks::SubmitTo(BossTank* bossTank)
	{
		SubmitMissileTask(bossTank);
		SubmitWaitTask(bossTank);
		SubmitRollingTask(bossTank);
		SubmitSummonTask(bossTank);
		SubmitRockTask(bossTank);
		SubmitChargeTask(bossTank);
	}

	void BossTankTasks::SubmitMissileTask(BossTank* bossTank)
	{
		EnemyTask MissileTask;

		//�~�T�C���𔭎˂��鏈��
		auto ShotMissile = [bossTank]()
		{
			EnemyMissile* missile = NewGO<EnemyMissile>(0, "missile");
			Vector3 pos = bossTank->GetPosition();
			pos.y += MISSILE_SHOT_HEIGHT;
			missile->SetPosition(pos);
		};

		MissileTask.SetUpdateFunc([bossTank,ShotMissile](int taskTime)
			{
				//�K��t���[���ɂȂ�ƃ~�T�C���𔭎˂���
				if (taskTime == MISSILE_TIME_FIRST)
				{
					ShotMissile();
				}

				if (taskTime == MISSILE_TIME_SECOND)
				{
					ShotMissile();
				}

				if (taskTime == MISSILE_TIME_THIRD)
				{
					ShotMissile();

					//�O���ڂ���������^�X�N�͏I��
					return true;
				}

				return false;
			}
		);

		//�{�X�ɂ��̃^�X�N���Z�b�g����
		bossTank->SetTask(enMissile, MissileTask);
	}

	void BossTankTasks::SubmitRollingTask(BossTank* bossTank)
	{
		EnemyTask RollingTask;

		RollingTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				//���t���[����������]�����Ă���
				bossTank->SetTurretDeg(bossTank->GetTurretDeg() + ROLLING_DEG);

				//�K��t���[���ɂȂ�����I��
				if (taskTime == ROLLING_TIME_END)
				{
					return true;
				}

				return false;
			}
		);

		//�^�X�N�̏I�����Ƀ_���[�W���������
		RollingTask.SetEndFunc([bossTank]()
			{
				//TODO:�����ł͂Ȃ��g���K�[�Ŕ��肵������
				Player* player = FindGO<Player>("player");
				Vector3 distance = player->GetPosition() - bossTank->GetPosition();

				//�v���C���[�Ƃ̋������߂�������
				if (distance.LengthSq() < ROLLING_RANGE * ROLLING_RANGE && player->isGuard() == false)
				{
					distance.Normalize();

					//�v���C���[��傫��������΂�
					player->KnockDown(distance * 60);
				}
			}
		);

		//�{�X�ɂ��̃^�X�N���Z�b�g����
		bossTank->SetTask(enRolling, RollingTask);
	}

	void BossTankTasks::SubmitSummonTask(BossTank* bossTank)
	{
		EnemyTask SummonTask;

		SummonTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				if (taskTime == SUMMON_TIME)
				{
					//�G���G������
					MiniEnemy* enemy[SUMMON_NUM];
					for (int i = 0; i < SUMMON_NUM; i++)
					{
						enemy[i] = NewGO<MiniEnemy>(0, "enemy");
						enemy[i]->SetPosition(MINION_POSITIONS[i]);
					}

					//����������^�X�N�͏I���
					return true;
				}

				return false;
			}
		);

		//�{�X�ɂ��̃^�X�N���Z�b�g����
		bossTank->SetTask(enSummon, SummonTask);
	}

	void BossTankTasks::SubmitChargeTask(BossTank* bossTank)
	{
		EnemyTask ChargeTask;

		ChargeTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				//���t���[����������]�����Ă���
				bossTank->SetBaseDeg(bossTank->GetBaseDeg() + ROLLING_DEG);

				//�K��t���[���ɂȂ�����I��
				if (taskTime == ROLLING_TIME_END)
				{
					return true;
				}

				return false;
			}
		);

		//�^�X�N�̏I�����Ƀ_���[�W���������
		ChargeTask.SetEndFunc([bossTank]()
			{
				//TODO:�����ł͂Ȃ��g���K�[�Ŕ��肵������
				Player* player = FindGO<Player>("player");
				Vector3 distance = player->GetPosition() - bossTank->GetPosition();

				//�v���C���[�Ƃ̋������߂�������
				if (distance.LengthSq() < ROLLING_RANGE * ROLLING_RANGE && player->isGuard() == false)
				{
					distance.Normalize();

					//�v���C���[��傫��������΂�
					player->KnockDown(distance * ROLLING_KNOCKDOWN_POWER);
				}
			}
		);

		//�{�X�ɂ��̃^�X�N���Z�b�g����
		bossTank->SetTask(enCharge, ChargeTask);
	}

	void BossTankTasks::SubmitRockTask(BossTank* bossTank)
	{
		EnemyTask RockTask;

		//����쐬����֐�
		auto ShotRock = [bossTank](const Vector3& pos) {
			EnemyRock* rock = NewGO<EnemyRock>(0, "rock");
			Player* player = FindGO<Player>("player");
			Vector3 distance = player->GetPosition() - bossTank->GetPosition();
			distance.Normalize();
			rock->SetPosition(pos);
			rock->SetDirection(distance);
		};

		RockTask.SetUpdateFunc([bossTank,ShotRock](int taskTime)
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
		bossTank->SetTask(enRock, RockTask);
	}

	void BossTankTasks::SubmitWaitTask(BossTank* bossTank)
	{
		EnemyTask WaitTask;

		WaitTask.SetUpdateFunc([](int taskTime)
			{
				if (taskTime == WAIT_TIME)
				{
					//�ҋ@���ԕ��ҋ@������^�X�N�͏I��
					return true;
				}
				else
				{
					return false;
				}
			}
		);

		//�{�X�ɂ��̃^�X�N���Z�b�g����
		bossTank->SetTask(enWait, WaitTask);
	}
}