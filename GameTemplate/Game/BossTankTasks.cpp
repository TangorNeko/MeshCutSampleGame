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
	const int SUMMON_EVENTCAMERA_START_TIME = 0;
	const int SUMMON_BOSSCAMERA_START_TIME = 100;
	const int SUMMON_EVENTCAMERA_END_TIME = 500;
	const int SUMMON_BOSSCAMERA_END_TIME = 400;
	const float SUMMON_BOSSCAMERA_START_DIVIDENUM = 70.0f;
	const int SUMMON_BOSSCAMERA_START_OFFSET = 30;
	const float SUMMON_BOSSCAMERA_END_DIVIDENUM = 100.0f;
	const float SUMMON_BOSSCAMERA_END_OFFSET = 500.0f;
	const float SUMMON_BACKHANDSPRING_POWER = 30.0f;
	const char16_t* SUMMON_EFFECT_PATH = u"Assets/effect/Teleport.efk";
	const int SUMMON_PLAYER_RESET_TIME = 10;
	const int SUMMON_TIME = 300;
	const int SUMMON_NUM = 4;
	const Vector3 MINION_POSITIONS[SUMMON_NUM] = { { 850.0f,0.0f,-1400.0f }, { -850.0f,0.0f,-1400.0f } ,{ 1300.0f,0.0f,-1000.0f } ,{ -1300.0f,0.0f,-1000.0f } };
	const float ROCK_SHOT_HEIGHT = 200.0f;
	const float ROCK_SHOT_XOFFSET = 100.0f;
	const int ROCK_TIME_FIRST = 40;
	const int ROCK_TIME_SECOND = 45;
	const int ROCK_TIME_THIRD = 48;
	const int WAIT_TIME = 150;
	const float STEP_START_TURRET_DEG = 0.0f;
	const int STEP_EVENTCAMERA_START_TIME = 30;
	const int STEP_BOSSCAMERA_START_TIME = 100;
	const int STEP_EVENTCAMERA_END_TIME = 325;
	const int STEP_BOSSCAMERA_END_TIME = 275;
	const int STEP_PLAYER_RESET_TIME = 140;
	const int STEP_MISSILE1_SHOT_TIME = 150;
	const int STEP_MISSILE2_SHOT_TIME = 250;
	const int STEP_MISSILE3_SHOT_TIME = 350;
	const int STEP_COMMAND_TIME = 325;
	const int STEP_PAUSE_TIME = 326;
	const float STEP_BOSSCAMERA_START_DIVIDENUM = 70.0f;
	const int STEP_BOSSCAMERA_START_OFFSET = 30;
	const float STEP_BOSSCAMERA_END_DIVIDENUM = 50.0f;
	const float STEP_BOSSCAMERA_END_OFFSET = 325.0f;
	const Vector3 PLAYER_RESET_POSITION = { 0.0f,0.0f,1200.0f };
	const Vector3 EVENT_CAMERA_POSITION = { 0.0f,200.0f,0.0f };
	const char16_t* EFFECT_WARNING_PATH = u"Assets/effect/Warning.efk";
	const Vector3 EFFECT_WARNING_HEIGHT = { 0.0f,250.0f,0.0f };
	const wchar_t* WARP_SOUND_PATH = L"Assets/sound/WarpSE.wav";
	const wchar_t* MISSILE_SOUND_PATH = L"Assets/sound/MissileLaunchSE.wav";
	const wchar_t* ROLLING_SOUND_PATH = L"Assets/sound/RollingSE.wav";
	const wchar_t* DAMAGE_SOUND_PATH = L"Assets/sound/DamageSE.wav";
}

namespace Game
{
	//TODO:�^�X�N���Ƃɕ���?

	void BossTankTasks::SubmitTo(BossTank* bossTank)
	{
		//�{�X�Ɋe�^�X�N��ݒ肷��
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
		bossTank->SetTask(enRolling, RollingTask);
	}

	void BossTankTasks::SubmitSummonTask(BossTank* bossTank)
	{
		EnemyTask SummonTask;

		SummonTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{

				//�C�x���g�J�����̊J�n�t���[�����Ȃ�
				if (taskTime <= SUMMON_BOSSCAMERA_START_TIME && taskTime >= SUMMON_EVENTCAMERA_START_TIME)
				{
					//�v���C���[���f���J��������{�X���f���J�����ɏ������ω������Ă���
					float t = (taskTime - SUMMON_BOSSCAMERA_START_OFFSET) / SUMMON_BOSSCAMERA_START_DIVIDENUM;
					Vector3 cameraPosition;
					Vector3 cameraTarget;
					Player* player = FindGO<Player>("player");
					cameraPosition.Lerp(t, player->GetCameraPosition(), EVENT_CAMERA_POSITION);
					cameraTarget.Lerp(t, player->GetCameraTarget(),bossTank->GetPosition());
					g_camera3D->SetPosition(cameraPosition);
					g_camera3D->SetTarget(cameraTarget);
				}

				//�{�X���f���J�����̃t���[�����Ȃ�
				if (taskTime > SUMMON_BOSSCAMERA_START_TIME && taskTime < SUMMON_BOSSCAMERA_END_TIME)
				{
					//�{�X���f��
					g_camera3D->SetPosition(EVENT_CAMERA_POSITION);
					g_camera3D->SetTarget(bossTank->GetPosition());
				}

				//�v���C���[�̈ʒu�����Z�b�g����t���[���Ȃ�
				if (taskTime == SUMMON_PLAYER_RESET_TIME)
				{
					Player* player = FindGO<Player>("player");
					Vector3 resPos = PLAYER_RESET_POSITION;
					Vector3 toResPos = resPos - player->GetPosition();
					toResPos.Normalize();

					//���Z�b�g������W�Ɍ������ăv���C���[����]������
					player->BackHandSpring(toResPos * SUMMON_BACKHANDSPRING_POWER);
				}

				//�G���G�̏����t���[���Ȃ�
				if (taskTime == SUMMON_TIME)
				{
					MiniEnemy* enemy[SUMMON_NUM];
					for (int i = 0; i < SUMMON_NUM; i++)
					{
						//�G���G������
						enemy[i] = NewGO<MiniEnemy>(Priority::High, "enemy");
						enemy[i]->SetPosition(MINION_POSITIONS[i]);

						//�o���G�t�F�N�g���Đ�
						Game::Effect* spawnEffect = NewGO<Game::Effect>(Priority::High);
						spawnEffect->SetPosition(MINION_POSITIONS[i]);
						spawnEffect->Init(SUMMON_EFFECT_PATH);
						spawnEffect->Play();
					}

					SoundOneShotPlay(WARP_SOUND_PATH);
					//����������^�X�N�͏I���
				}

				//�C�x���g�J�����̏I���t���[�����Ȃ�
				if (taskTime >= SUMMON_BOSSCAMERA_END_TIME && taskTime <= SUMMON_EVENTCAMERA_END_TIME)
				{
					//�{�X���f���J��������v���C���[���f���J�����ɏ������ω������Ă���
					float t = (SUMMON_BOSSCAMERA_END_OFFSET - taskTime) / SUMMON_BOSSCAMERA_END_DIVIDENUM;
					Vector3 cameraPosition;
					Vector3 cameraTarget;
					Player* player = FindGO<Player>("player");
					cameraPosition.Lerp(t, player->GetCameraPosition(), EVENT_CAMERA_POSITION);
					cameraTarget.Lerp(t, player->GetCameraTarget(), bossTank->GetPosition());
					g_camera3D->SetPosition(cameraPosition);
					g_camera3D->SetTarget(cameraTarget);
				}

				//�C�x���g�̃J�����̍ŏI�t���[���Ȃ�
				if (taskTime == SUMMON_EVENTCAMERA_END_TIME)
				{
					//�^�X�N�̏I��
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
				Effect* warningEffect = NewGO<Effect>(Priority::High);
				warningEffect->Init(EFFECT_WARNING_PATH);
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
					if (distance.LengthSq() < ROLLING_RANGE * ROLLING_RANGE)
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

		StepTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				//�C�x���g�J�����̊J�n�t���[�����Ȃ�
				if (taskTime <= STEP_BOSSCAMERA_START_TIME && taskTime >= STEP_EVENTCAMERA_START_TIME)
				{
					bossTank->SetTurretDeg(STEP_START_TURRET_DEG);
					float t = (taskTime - STEP_BOSSCAMERA_START_OFFSET) / STEP_BOSSCAMERA_START_DIVIDENUM;
					Vector3 cameraPosition;
					Vector3 cameraTarget;
					Player* player = FindGO<Player>("player");
					cameraPosition.Lerp(t, player->GetCameraPosition(), EVENT_CAMERA_POSITION);
					cameraTarget.Lerp(t, player->GetCameraTarget(), bossTank->GetPosition());
					player->SetPlayerDirection(Vector3::Back);
					g_camera3D->SetPosition(cameraPosition);
					g_camera3D->SetTarget(cameraTarget);
				}

				//�{�X���f���J�����̃t���[�����Ȃ�
				if (taskTime > STEP_BOSSCAMERA_START_TIME && taskTime < STEP_BOSSCAMERA_END_TIME)
				{
					g_camera3D->SetPosition(EVENT_CAMERA_POSITION);
					g_camera3D->SetTarget(bossTank->GetPosition());
				}

				//�v���C���[�̈ʒu�����Z�b�g����t���[���Ȃ�
				if (taskTime == STEP_PLAYER_RESET_TIME)
				{
					Vector3 resPos = PLAYER_RESET_POSITION;
					Player* player = FindGO<Player>("player");
					player->SetPosition(resPos);
				}

				//����~�T�C���𔭎˂��鏈��
				auto ShotStepMissile = [bossTank](const Vector3& position)
				{
					//�{�X����v���C���[�ւ̌������擾
					Player* player = FindGO<Player>("player");
					Vector3 direction = player->GetPosition() - bossTank->GetPosition();
					direction.Normalize();

					//����~�T�C�����쐬
					StepObject* stepObject = NewGO<StepObject>(Priority::High, "stepObject");

					//����~�T�C���̍��W�ƌ�����ݒ�
					stepObject->SetPosition(position);
					stepObject->SetMoveDirection(direction);

					SoundOneShotPlay(MISSILE_SOUND_PATH);
				};

				//�K��t���[���ɂȂ����瑫��~�T�C���𔭎�
				if (taskTime == STEP_MISSILE1_SHOT_TIME)
				{
					ShotStepMissile(bossTank->GetRightCannonPosition());
				}

				if (taskTime == STEP_MISSILE2_SHOT_TIME)
				{
					ShotStepMissile(bossTank->GetLeftCannonPosition());
				}

				if (taskTime == STEP_MISSILE3_SHOT_TIME)
				{
					ShotStepMissile(bossTank->GetRightCannonPosition());
				}

				//�C�x���g�J�����̏I���t���[�����Ȃ�
				if (taskTime >= STEP_BOSSCAMERA_END_TIME &&  taskTime <= STEP_EVENTCAMERA_END_TIME)
				{
					float t = (STEP_BOSSCAMERA_END_OFFSET - taskTime) / STEP_BOSSCAMERA_END_DIVIDENUM;
					Vector3 cameraPosition;
					Vector3 cameraTarget;
					Player* player = FindGO<Player>("player");
					cameraPosition.Lerp(t, player->GetCameraPosition(), EVENT_CAMERA_POSITION);
					cameraTarget.Lerp(t, player->GetCameraTarget(), bossTank->GetPosition());
					g_camera3D->SetPosition(cameraPosition);
					g_camera3D->SetTarget(cameraTarget);
				}

				//�R�}���h���͂�\������t���[���Ȃ�
				if (taskTime == STEP_COMMAND_TIME)
				{
					//�R�}���h���͂��쐬
					NewGO<CommandInput>(Priority::High);

					//�v���C���[�Ƀ~�T�C���ړ����n�܂�������ʒm
					Player* player = FindGO<Player>("player");
					player->NoticeMissileMoveStart();
				}

				//�R�}���h���͂��󂯕t���邽�߃Q�[�����|�[�Y����t���[���Ȃ�
				if (taskTime == STEP_PAUSE_TIME)
				{
					//�Q�[�����|�[�Y������
					GameObjectManager::GetInstance()->SetPauseFlag(true);
				}

				//�v���C���[���C�g��ؒf����ƃ^�X�N���������f����邽�߂���܂ŏI�����Ȃ��B
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
		bossTank->SetTask(enEventRolling, EventRollingTask);
	}
}