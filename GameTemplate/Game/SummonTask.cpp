#include "stdafx.h"
#include "SummonTask.h"
#include "BossTank.h"
#include "Player.h"
#include "MiniEnemy.h"

namespace
{
	const int SUMMON_EVENTCAMERA_START_TIME = 0;						//カメラがボスに移動し始める開始フレーム
	const int SUMMON_BOSSCAMERA_START_TIME = 100;						//カメラがボスを映す開始フレーム
	const int SUMMON_BOSSCAMERA_END_TIME = 400;							//カメラがボスを映す終了フレーム
	const int SUMMON_EVENTCAMERA_END_TIME = 500;						//カメラがプレイヤーに移動し終わるフレーム
	const float SUMMON_BOSSCAMERA_START_DIVIDENUM = 70.0f;				//カメラがボスに移動する時の移動量の除算値
	const int SUMMON_BOSSCAMERA_START_OFFSET = 30;						//カメラがボスに移動する時の移動量のオフセット値
	const float SUMMON_BOSSCAMERA_END_DIVIDENUM = 100.0f;				//カメラがプレイヤーに移動する時の移動量の除算値
	const float SUMMON_BOSSCAMERA_END_OFFSET = 500.0f;					//カメラがプレイヤーに移動する時の移動量のオフセット値
	const float SUMMON_BACKHANDSPRING_POWER = 30.0f;					//プレイヤーの後転の移動力
	const int SUMMON_PLAYER_RESET_TIME = 10;							//プレイヤーの位置をリセットするフレーム
	const int SUMMON_TIME = 300;										//雑魚敵を召喚するフレーム
	const int SUMMON_NUM = 4;											//雑魚敵の召喚量
	const Vector3 MINION_POSITIONS[SUMMON_NUM] =						//雑魚敵の召喚座標
	{
		{ 850.0f,0.0f,-1400.0f },
		{ -850.0f,0.0f,-1400.0f } ,
		{ 1300.0f,0.0f,-1000.0f } ,
		{ -1300.0f,0.0f,-1000.0f }
	};
	const Vector3 PLAYER_RESET_POSITION = { 0.0f,0.0f,1200.0f };		//プレイヤーの位置をリセットする座標
	const Vector3 EVENT_CAMERA_POSITION = { 0.0f,200.0f,0.0f };			//カメラがボスを映す時のカメラ座標
	const wchar_t* WARP_SOUND_PATH = L"Assets/sound/WarpSE.wav";		//召喚時の効果音パス
	const char16_t* SUMMON_EFFECT_PATH = u"Assets/effect/Teleport.efk";	//召喚時のエフェクトパス
}

namespace Game
{
	void SummonTask::SubmitTo(BossTank* bossTank, int taskNum)
	{
		EnemyTask SummonTask;

		SummonTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{

				//イベントカメラの開始フレーム中なら
				if (taskTime <= SUMMON_BOSSCAMERA_START_TIME && taskTime >= SUMMON_EVENTCAMERA_START_TIME)
				{
					//プレイヤーを映すカメラからボスを映すカメラに少しずつ変化させていく
					float t = (taskTime - SUMMON_BOSSCAMERA_START_OFFSET) / SUMMON_BOSSCAMERA_START_DIVIDENUM;
					Vector3 cameraPosition;
					Vector3 cameraTarget;
					Player* player = FindGO<Player>("player");
					cameraPosition.Lerp(t, player->GetCameraPosition(), EVENT_CAMERA_POSITION);
					cameraTarget.Lerp(t, player->GetCameraTarget(), bossTank->GetPosition());
					g_camera3D->SetPosition(cameraPosition);
					g_camera3D->SetTarget(cameraTarget);
				}

				//ボスを映すカメラのフレーム中なら
				if (taskTime > SUMMON_BOSSCAMERA_START_TIME && taskTime < SUMMON_BOSSCAMERA_END_TIME)
				{
					//ボスを映す
					g_camera3D->SetPosition(EVENT_CAMERA_POSITION);
					g_camera3D->SetTarget(bossTank->GetPosition());
				}

				//プレイヤーの位置をリセットするフレームなら
				if (taskTime == SUMMON_PLAYER_RESET_TIME)
				{
					Player* player = FindGO<Player>("player");
					Vector3 resPos = PLAYER_RESET_POSITION;
					Vector3 toResPos = resPos - player->GetPosition();
					toResPos.Normalize();

					//リセットする座標に向かってプレイヤーを後転させる
					player->BackHandSpring(toResPos * SUMMON_BACKHANDSPRING_POWER);
				}

				//雑魚敵の召喚フレームなら
				if (taskTime == SUMMON_TIME)
				{
					MiniEnemy* enemy[SUMMON_NUM];
					for (int i = 0; i < SUMMON_NUM; i++)
					{
						//雑魚敵を召喚
						enemy[i] = NewGO<MiniEnemy>(Priority::High, "enemy");
						enemy[i]->SetPosition(MINION_POSITIONS[i]);

						//出現エフェクトを再生
						Game::Effect* spawnEffect = NewGO<Game::Effect>(Priority::High);
						spawnEffect->SetPosition(MINION_POSITIONS[i]);
						spawnEffect->Init(SUMMON_EFFECT_PATH);
						spawnEffect->Play();
					}

					SoundOneShotPlay(WARP_SOUND_PATH);
					//召喚したらタスクは終わり
				}

				//イベントカメラの終了フレーム中なら
				if (taskTime >= SUMMON_BOSSCAMERA_END_TIME && taskTime <= SUMMON_EVENTCAMERA_END_TIME)
				{
					//ボスを映すカメラからプレイヤーを映すカメラに少しずつ変化させていく
					float t = (SUMMON_BOSSCAMERA_END_OFFSET - taskTime) / SUMMON_BOSSCAMERA_END_DIVIDENUM;
					Vector3 cameraPosition;
					Vector3 cameraTarget;
					Player* player = FindGO<Player>("player");
					cameraPosition.Lerp(t, player->GetCameraPosition(), EVENT_CAMERA_POSITION);
					cameraTarget.Lerp(t, player->GetCameraTarget(), bossTank->GetPosition());
					g_camera3D->SetPosition(cameraPosition);
					g_camera3D->SetTarget(cameraTarget);
				}

				//イベントのカメラの最終フレームなら
				if (taskTime == SUMMON_EVENTCAMERA_END_TIME)
				{
					//タスクの終了
					return true;
				}

				return false;
			}
		);

		//ボスにこのタスクをセットする
		bossTank->SetTask(taskNum, SummonTask);
	}
}