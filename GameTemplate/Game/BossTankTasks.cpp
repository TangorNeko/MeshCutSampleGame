#include "stdafx.h"
#include "BossTankTasks.h"
#include "BossTank.h"
#include "EnemyMissile.h"
#include "Player.h"
#include "MiniEnemy.h"
#include "EnemyRock.h"
#include "StepObject.h"
#include "CommandInput.h"

namespace
{
	const int MISSILE_TIME_FIRST = 40;
	const int MISSILE_TIME_SECOND = 70;
	const int MISSILE_TIME_THIRD = 100;
	const float MISSILE_SHOT_HEIGHT = 250.0f;
	const Vector3 MISSILE_TO_RIGHTCANNON = { 160.0f,250.0f,0.0f };
	const Vector3 MISSILE_TO_LEFTCANNON = { -160.0f,250.0f,0.0f };
	const float ROLLING_DEG = 15.0f;
	const int ROLLING_TIME_START = 40;
	const int ROLLING_TIME_END = 63;
	const float ROLLING_RANGE = 700.0f;
	const float ROLLING_KNOCKDOWN_POWER = 60.0f;
	const int SUMMON_TIME = 300;
	const int SUMMON_NUM = 4;
	const Vector3 MINION_POSITIONS[SUMMON_NUM] = { { 850.0f,0.0f,-1400.0f }, { -850.0f,0.0f,-1400.0f } ,{ 1300.0f,0.0f,-1000.0f } ,{ -1300.0f,0.0f,-1000.0f } };
	const float ROCK_SHOT_HEIGHT = 200.0f;
	const float ROCK_SHOT_XOFFSET = 100.0f;
	const int ROCK_TIME_FIRST = 40;
	const int ROCK_TIME_SECOND = 45;
	const int ROCK_TIME_THIRD = 48;
	const int WAIT_TIME = 150;
	const Vector3 EVENT_CAMERA_POSITION = { 0.0f,200.0f,0.0f };
	const Vector3 EFFECT_WARNING_HEIGHT = { 0.0f,250.0f,0.0f };
	const wchar_t* WARP_SOUND_PATH = L"Assets/sound/WarpSE.wav";
	const wchar_t* MISSILE_SOUND_PATH = L"Assets/sound/MissileLaunchSE.wav";
	const wchar_t* ROLLING_SOUND_PATH = L"Assets/sound/RollingSE.wav";
	const wchar_t* DAMAGE_SOUND_PATH = L"Assets/sound/DamageSE.wav";
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
		SubmitStepTask(bossTank);
		SubmitEventRollingTask(bossTank);
	}

	void BossTankTasks::SubmitMissileTask(BossTank* bossTank)
	{
		EnemyTask MissileTask;

		//�~�T�C���𔭎˂��鏈��
		auto ShotMissile = [bossTank](const Vector3& position)
		{
			EnemyMissile* missile = NewGO<EnemyMissile>(0, "missile");
			missile->SetPosition(position);

			SoundOneShotPlay(MISSILE_SOUND_PATH);
		};

		MissileTask.SetStartFunc([bossTank]()
			{
				Effect* warningEffect = NewGO<Effect>(0);
				warningEffect->Init(u"Assets/effect/Warning.efk");
				warningEffect->SetPosition(bossTank->GetPosition() + EFFECT_WARNING_HEIGHT);
				warningEffect->Play();
			}
		);

		MissileTask.SetUpdateFunc([bossTank,ShotMissile](int taskTime)
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
		bossTank->SetTask(enMissile, MissileTask);
	}

	void BossTankTasks::SubmitRollingTask(BossTank* bossTank)
	{
		EnemyTask RollingTask;

		RollingTask.SetStartFunc([bossTank]()
			{
				Effect* warningEffect = NewGO<Effect>(0);
				warningEffect->Init(u"Assets/effect/Warning.efk");
				warningEffect->SetPosition(bossTank->GetPosition() + EFFECT_WARNING_HEIGHT);
				warningEffect->Play();
			}
		);

		RollingTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				if (taskTime == ROLLING_TIME_START)
				{
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
					//TODO:�����ł͂Ȃ��g���K�[�Ŕ��肵������
					Player* player = FindGO<Player>("player");
					Vector3 distance = player->GetPosition() - bossTank->GetPosition();

					//�v���C���[�Ƃ̋������߂�������
					if (distance.LengthSq() < ROLLING_RANGE * ROLLING_RANGE && player->isGuard() == false)
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

		//�^�X�N�̏I�����Ƀ_���[�W���������
		RollingTask.SetEndFunc([bossTank]()
			{
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

				if (taskTime <= 100 && taskTime >= 0)
				{
					float t = (taskTime - 30) / 70.0f;
					Vector3 cameraPosition;
					Vector3 cameraTarget;
					Player* player = FindGO<Player>("player");
					cameraPosition.Lerp(t, player->GetCameraPosition(), EVENT_CAMERA_POSITION);
					cameraTarget.Lerp(t, player->GetCameraTarget(),bossTank->GetPosition());
					g_camera3D->SetPosition(cameraPosition);
					g_camera3D->SetTarget(cameraTarget);
				}

				if (taskTime > 100 && taskTime < 400)
				{
					g_camera3D->SetPosition(EVENT_CAMERA_POSITION);
					g_camera3D->SetTarget(bossTank->GetPosition());
				}

				if (taskTime == 10)
				{
					Player* player = FindGO<Player>("player");

					Vector3 resPos = { 0.0f,0.0f,1200.0f };

					Vector3 toResPos = resPos - player->GetPosition();

					toResPos.Normalize();

					player->BackHandSpring(toResPos * 30);
				}

				if (taskTime == SUMMON_TIME)
				{
					//�G���G������
					MiniEnemy* enemy[SUMMON_NUM];
					for (int i = 0; i < SUMMON_NUM; i++)
					{
						enemy[i] = NewGO<MiniEnemy>(0, "enemy");
						enemy[i]->SetPosition(MINION_POSITIONS[i]);

						Game::Effect* spawnEffect = NewGO<Game::Effect>(1);
						spawnEffect->SetPosition(MINION_POSITIONS[i]);
						spawnEffect->Init(u"Assets/effect/Teleport.efk");
						spawnEffect->Play();
					}

					SoundOneShotPlay(WARP_SOUND_PATH);
					//����������^�X�N�͏I���
				}

				if (taskTime >= 400 && taskTime <= 500)
				{
					float t = (500.0f - taskTime) / 100.0f;
					Vector3 cameraPosition;
					Vector3 cameraTarget;
					Player* player = FindGO<Player>("player");
					cameraPosition.Lerp(t, player->GetCameraPosition(), EVENT_CAMERA_POSITION);
					cameraTarget.Lerp(t, player->GetCameraTarget(), bossTank->GetPosition());
					g_camera3D->SetPosition(cameraPosition);
					g_camera3D->SetTarget(cameraTarget);
				}

				if (taskTime == 500)
				{
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

		ChargeTask.SetStartFunc([bossTank]()
			{
				Effect* warningEffect = NewGO<Effect>(0);
				warningEffect->Init(u"Assets/effect/Warning.efk");
				warningEffect->SetPosition(bossTank->GetPosition() + EFFECT_WARNING_HEIGHT);
				warningEffect->Play();
			}
		);

		ChargeTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				if (taskTime >= ROLLING_TIME_START)
				{
					//���t���[����������]�����Ă���
					bossTank->SetBaseDeg(bossTank->GetBaseDeg() + ROLLING_DEG);
				}

				//�K��t���[���ɂȂ�����I��
				if (taskTime == ROLLING_TIME_END)
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
					return true;
				}

				return false;
			}
		);

		//�^�X�N�̏I�����Ƀ_���[�W���������
		ChargeTask.SetEndFunc([bossTank]()
			{
			}
		);

		//�{�X�ɂ��̃^�X�N���Z�b�g����
		bossTank->SetTask(enCharge, ChargeTask);
	}

	void BossTankTasks::SubmitRockTask(BossTank* bossTank)
	{
		EnemyTask RockTask;

		RockTask.SetStartFunc([bossTank]()
			{
				Effect* warningEffect = NewGO<Effect>(0);
				warningEffect->Init(u"Assets/effect/Warning.efk");
				warningEffect->SetPosition(bossTank->GetPosition() + EFFECT_WARNING_HEIGHT);
				warningEffect->Play();
			}
		);

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

	void BossTankTasks::SubmitStepTask(BossTank* bossTank)
	{
		EnemyTask StepTask;

		//TODO:���A�}�W�b�N�i���o�[���炯�Ȃ̂Ő�������
		StepTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{

				if (taskTime <= 100 && taskTime >= 30)
				{
					bossTank->SetTurretDeg(0.0f);
					float t = (taskTime - 30) / 70.0f;
					Vector3 cameraPosition;
					Vector3 cameraTarget;
					Player* player = FindGO<Player>("player");
					cameraPosition.Lerp(t, player->GetCameraPosition(), EVENT_CAMERA_POSITION);
					cameraTarget.Lerp(t, player->GetCameraTarget(), bossTank->GetPosition());
					player->SetPlayerDirection(Vector3::Back);
					g_camera3D->SetPosition(cameraPosition);
					g_camera3D->SetTarget(cameraTarget);
				}

				if (taskTime > 100 && taskTime < 275)
				{
					g_camera3D->SetPosition(EVENT_CAMERA_POSITION);
					g_camera3D->SetTarget(bossTank->GetPosition());
				}

				if (taskTime == 140)
				{
					Player* player = FindGO<Player>("player");

					Vector3 resPos = { 0.0f,0.0f,1200.0f };

					player->SetPosition(resPos);
				}

				if (taskTime == 150)
				{
					Player* player = FindGO<Player>("player");
					Vector3 direction = player->GetPosition() - bossTank->GetPosition();
					direction.Normalize();

					StepObject* stepObject = NewGO<StepObject>(0, "stepObject");

					Vector3 position = bossTank->GetRightCannonPosition();
					stepObject->SetPosition(position);
					stepObject->SetMoveDirection(direction);

					SoundOneShotPlay(MISSILE_SOUND_PATH);
				}


				if (taskTime == 250)
				{
					Player* player = FindGO<Player>("player");
					Vector3 direction = player->GetPosition() - bossTank->GetPosition();
					direction.Normalize();

					StepObject* stepObject = NewGO<StepObject>(0, "stepObject");

					Vector3 position = bossTank->GetLeftCannonPosition();
					stepObject->SetPosition(position);
					stepObject->SetMoveDirection(direction);

					SoundOneShotPlay(MISSILE_SOUND_PATH);
				}

				if (taskTime == 350)
				{
					Player* player = FindGO<Player>("player");
					Vector3 direction = player->GetPosition() - bossTank->GetPosition();
					direction.Normalize();

					StepObject* stepObject = NewGO<StepObject>(0, "stepObject");

					Vector3 position = bossTank->GetRightCannonPosition();
					stepObject->SetPosition(position);
					stepObject->SetMoveDirection(direction);

					SoundOneShotPlay(MISSILE_SOUND_PATH);
				}

				if (taskTime >= 275 &&  taskTime <= 325)
				{
					float t = (325.0f - taskTime) / 50.0f;
					Vector3 cameraPosition;
					Vector3 cameraTarget;
					Player* player = FindGO<Player>("player");
					cameraPosition.Lerp(t, player->GetCameraPosition(), EVENT_CAMERA_POSITION);
					cameraTarget.Lerp(t, player->GetCameraTarget(), bossTank->GetPosition());
					g_camera3D->SetPosition(cameraPosition);
					g_camera3D->SetTarget(cameraTarget);
				}

				if (taskTime == 325)
				{
					NewGO<CommandInput>(0);
					Player* player = FindGO<Player>("player");
					player->NoticeMissileMoveStart();
				}

				if (taskTime == 326)
				{
					GameObjectManager::GetInstance()->SetPauseFlag(true);
				}


				if (taskTime == 400)
				{
					return true;
				}

				return false;
			}
		);

		bossTank->SetTask(enStep,StepTask);
	}

	void BossTankTasks::SubmitEventRollingTask(BossTank* bossTank)
	{
		EnemyTask EventRollingTask;

		EventRollingTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				if (taskTime == ROLLING_TIME_START)
				{
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
					//TODO:�����ł͂Ȃ��g���K�[�Ŕ��肵������
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
		bossTank->SetTask(enEventRolling, EventRollingTask);
	}
}