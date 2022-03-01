#include "stdafx.h"
#include "SummonTask.h"
#include "BossTank.h"
#include "Player.h"
#include "MiniEnemy.h"

namespace
{
	const int SUMMON_EVENTCAMERA_START_TIME = 0;						//�J�������{�X�Ɉړ����n�߂�J�n�t���[��
	const int SUMMON_BOSSCAMERA_START_TIME = 100;						//�J�������{�X���f���J�n�t���[��
	const int SUMMON_BOSSCAMERA_END_TIME = 400;							//�J�������{�X���f���I���t���[��
	const int SUMMON_EVENTCAMERA_END_TIME = 500;						//�J�������v���C���[�Ɉړ����I���t���[��
	const float SUMMON_BOSSCAMERA_START_DIVIDENUM = 70.0f;				//�J�������{�X�Ɉړ����鎞�̈ړ��ʂ̏��Z�l
	const int SUMMON_BOSSCAMERA_START_OFFSET = 30;						//�J�������{�X�Ɉړ����鎞�̈ړ��ʂ̃I�t�Z�b�g�l
	const float SUMMON_BOSSCAMERA_END_DIVIDENUM = 100.0f;				//�J�������v���C���[�Ɉړ����鎞�̈ړ��ʂ̏��Z�l
	const float SUMMON_BOSSCAMERA_END_OFFSET = 500.0f;					//�J�������v���C���[�Ɉړ����鎞�̈ړ��ʂ̃I�t�Z�b�g�l
	const float SUMMON_BACKHANDSPRING_POWER = 30.0f;					//�v���C���[�̌�]�̈ړ���
	const int SUMMON_PLAYER_RESET_TIME = 10;							//�v���C���[�̈ʒu�����Z�b�g����t���[��
	const int SUMMON_TIME = 300;										//�G���G����������t���[��
	const int SUMMON_NUM = 4;											//�G���G�̏�����
	const Vector3 MINION_POSITIONS[SUMMON_NUM] =						//�G���G�̏������W
	{
		{ 850.0f,0.0f,-1400.0f },
		{ -850.0f,0.0f,-1400.0f } ,
		{ 1300.0f,0.0f,-1000.0f } ,
		{ -1300.0f,0.0f,-1000.0f }
	};
	const Vector3 PLAYER_RESET_POSITION = { 0.0f,0.0f,1200.0f };		//�v���C���[�̈ʒu�����Z�b�g������W
	const Vector3 EVENT_CAMERA_POSITION = { 0.0f,200.0f,0.0f };			//�J�������{�X���f�����̃J�������W
	const wchar_t* WARP_SOUND_PATH = L"Assets/sound/WarpSE.wav";		//�������̌��ʉ��p�X
	const char16_t* SUMMON_EFFECT_PATH = u"Assets/effect/Teleport.efk";	//�������̃G�t�F�N�g�p�X
}

namespace Game
{
	void SummonTask::SubmitTo(BossTank* bossTank, int taskNum)
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
					cameraTarget.Lerp(t, player->GetCameraTarget(), bossTank->GetPosition());
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
		bossTank->SetTask(taskNum, SummonTask);
	}
}