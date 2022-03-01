#include "stdafx.h"
#include "StepTask.h"
#include "BossTank.h"
#include "Player.h"
#include "StepObject.h"
#include "CommandInput.h"

namespace
{
	const float STEP_START_TURRET_DEG = 0.0f;									//タスク開始時の砲塔の角度
	const int STEP_EVENTCAMERA_START_TIME = 30;									//カメラがボスに移動し始める開始フレーム
	const int STEP_BOSSCAMERA_START_TIME = 100;									//カメラがボスを映す開始フレーム
	const int STEP_BOSSCAMERA_END_TIME = 275;									//カメラがボスを映す終了フレーム
	const int STEP_EVENTCAMERA_END_TIME = 325;									//カメラがプレイヤーに移動し終わるフレーム
	const int STEP_PLAYER_RESET_TIME = 140;										//プレイヤーの位置をリセットするフレーム
	const int STEP_MISSILE1_SHOT_TIME = 150;									//1発目の足場ミサイルを発射するフレーム
	const int STEP_MISSILE2_SHOT_TIME = 250;									//1発目の足場ミサイルを発射するフレーム
	const int STEP_MISSILE3_SHOT_TIME = 350;									//1発目の足場ミサイルを発射するフレーム
	const int STEP_COMMAND_TIME = 325;											//コマンド入力を表示するフレーム
	const int STEP_PAUSE_TIME = 326;											//ゲームをポーズさせるフレーム
	const float STEP_BOSSCAMERA_START_DIVIDENUM = 70.0f;						//カメラがボスに移動される時の移動量の除算値
	const int STEP_BOSSCAMERA_START_OFFSET = 30;								//カメラがボスに移動される時の移動量のオフセット値
	const float STEP_BOSSCAMERA_END_DIVIDENUM = 50.0f;							//カメラがプレイヤーに移動する時の移動量の除算値
	const float STEP_BOSSCAMERA_END_OFFSET = 325.0f;							//カメラがプレイヤーに移動する時の移動量のオフセット値
	const Vector3 PLAYER_RESET_POSITION = { 0.0f,0.0f,1200.0f };				//プレイヤーの位置をリセットする座標
	const Vector3 EVENT_CAMERA_POSITION = { 0.0f,200.0f,0.0f };					//カメラがボスを映す時のカメラ座標
	const wchar_t* MISSILE_SOUND_PATH = L"Assets/sound/MissileLaunchSE.wav";	//ミサイル発射の効果音パス
}

namespace Game
{
	void StepTask::SubmitTo(BossTank* bossTank, int taskNum)
	{
		EnemyTask StepTask;

		StepTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				//イベントカメラの開始フレーム中なら
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

				//ボスを映すカメラのフレーム中なら
				if (taskTime > STEP_BOSSCAMERA_START_TIME && taskTime < STEP_BOSSCAMERA_END_TIME)
				{
					g_camera3D->SetPosition(EVENT_CAMERA_POSITION);
					g_camera3D->SetTarget(bossTank->GetPosition());
				}

				//プレイヤーの位置をリセットするフレームなら
				if (taskTime == STEP_PLAYER_RESET_TIME)
				{
					Vector3 resPos = PLAYER_RESET_POSITION;
					Player* player = FindGO<Player>("player");
					player->SetPosition(resPos);
				}

				//足場ミサイルを発射する処理
				auto ShotStepMissile = [bossTank](const Vector3& position)
				{
					//ボスからプレイヤーへの向きを取得
					Player* player = FindGO<Player>("player");
					Vector3 direction = player->GetPosition() - bossTank->GetPosition();
					direction.Normalize();

					//足場ミサイルを作成
					StepObject* stepObject = NewGO<StepObject>(Priority::High, "stepObject");

					//足場ミサイルの座標と向きを設定
					stepObject->SetPosition(position);
					stepObject->SetMoveDirection(direction);

					SoundOneShotPlay(MISSILE_SOUND_PATH);
				};

				//規定フレームになったら足場ミサイルを発射
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

				//イベントカメラの終了フレーム中なら
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

				//コマンド入力を表示するフレームなら
				if (taskTime == STEP_COMMAND_TIME)
				{
					//コマンド入力を作成
					NewGO<CommandInput>(Priority::High);

					//プレイヤーにミサイル移動が始まった事を通知
					Player* player = FindGO<Player>("player");
					player->NoticeMissileMoveStart();
				}

				//コマンド入力を受け付けるためゲームをポーズするフレームなら
				if (taskTime == STEP_PAUSE_TIME)
				{
					//ゲームをポーズさせる
					GameObjectManager::GetInstance()->SetPauseFlag(true);
				}

				//プレイヤーが砲身を切断するとタスクが強制中断されるためそれまで終了しない。
				return false;
			}
		);

		bossTank->SetTask(taskNum, StepTask);
	}
}