#include "stdafx.h"
#include "StepTask.h"
#include "BossTank.h"
#include "Player.h"
#include "StepObject.h"
#include "CommandInput.h"

namespace
{
	const float STEP_START_TURRET_DEG = 0.0f;									//�^�X�N�J�n���̖C���̊p�x
	const int STEP_EVENTCAMERA_START_TIME = 30;									//�J�������{�X�Ɉړ����n�߂�J�n�t���[��
	const int STEP_BOSSCAMERA_START_TIME = 100;									//�J�������{�X���f���J�n�t���[��
	const int STEP_BOSSCAMERA_END_TIME = 275;									//�J�������{�X���f���I���t���[��
	const int STEP_EVENTCAMERA_END_TIME = 325;									//�J�������v���C���[�Ɉړ����I���t���[��
	const int STEP_PLAYER_RESET_TIME = 140;										//�v���C���[�̈ʒu�����Z�b�g����t���[��
	const int STEP_MISSILE1_SHOT_TIME = 150;									//1���ڂ̑���~�T�C���𔭎˂���t���[��
	const int STEP_MISSILE2_SHOT_TIME = 250;									//1���ڂ̑���~�T�C���𔭎˂���t���[��
	const int STEP_MISSILE3_SHOT_TIME = 350;									//1���ڂ̑���~�T�C���𔭎˂���t���[��
	const int STEP_COMMAND_TIME = 325;											//�R�}���h���͂�\������t���[��
	const int STEP_PAUSE_TIME = 326;											//�Q�[�����|�[�Y������t���[��
	const float STEP_BOSSCAMERA_START_DIVIDENUM = 70.0f;						//�J�������{�X�Ɉړ�����鎞�̈ړ��ʂ̏��Z�l
	const int STEP_BOSSCAMERA_START_OFFSET = 30;								//�J�������{�X�Ɉړ�����鎞�̈ړ��ʂ̃I�t�Z�b�g�l
	const float STEP_BOSSCAMERA_END_DIVIDENUM = 50.0f;							//�J�������v���C���[�Ɉړ����鎞�̈ړ��ʂ̏��Z�l
	const float STEP_BOSSCAMERA_END_OFFSET = 325.0f;							//�J�������v���C���[�Ɉړ����鎞�̈ړ��ʂ̃I�t�Z�b�g�l
	const Vector3 PLAYER_RESET_POSITION = { 0.0f,0.0f,1200.0f };				//�v���C���[�̈ʒu�����Z�b�g������W
	const Vector3 EVENT_CAMERA_POSITION = { 0.0f,200.0f,0.0f };					//�J�������{�X���f�����̃J�������W
	const wchar_t* MISSILE_SOUND_PATH = L"Assets/sound/MissileLaunchSE.wav";	//�~�T�C�����˂̌��ʉ��p�X
}

namespace Game
{
	void StepTask::SubmitTo(BossTank* bossTank, int taskNum)
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
				if (taskTime >= STEP_BOSSCAMERA_END_TIME && taskTime <= STEP_EVENTCAMERA_END_TIME)
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

		bossTank->SetTask(taskNum, StepTask);
	}
}