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
	//TODO:タスクごとに分離?

	void BossTankTasks::SubmitTo(BossTank* bossTank)
	{
		//ボスに各タスクを設定する
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

		//ミサイルを発射する処理
		auto ShotMissile = [bossTank](const Vector3& position)
		{
			//ミサイルを作成
			EnemyMissile* missile = NewGO<EnemyMissile>(Priority::High, "missile");
			missile->SetPosition(position);

			//発射サウンドを再生
			SoundOneShotPlay(MISSILE_SOUND_PATH);
		};

		MissileTask.SetStartFunc([bossTank]()
			{
				//警告エフェクトを再生
				Effect* warningEffect = NewGO<Effect>(Priority::High);
				warningEffect->Init(EFFECT_WARNING_PATH);
				warningEffect->SetPosition(bossTank->GetPosition() + EFFECT_WARNING_HEIGHT);
				warningEffect->Play();
			}
		);

		MissileTask.SetUpdateFunc([bossTank,ShotMissile](int taskTime)
			{
				//規定フレームになるとミサイルを発射する
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

					//三発目を撃ったらタスクは終了
					return true;
				}

				return false;
			}
		);

		//ボスにこのタスクをセットする
		bossTank->SetTask(enMissile, MissileTask);
	}

	void BossTankTasks::SubmitRollingTask(BossTank* bossTank)
	{
		EnemyTask RollingTask;

		RollingTask.SetStartFunc([bossTank]()
			{
				//警告エフェクトを再生
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
					//回転サウンドを再生
					SoundOneShotPlay(ROLLING_SOUND_PATH);
				}

				if (taskTime >= ROLLING_TIME_START)
				{
					//毎フレーム少しずつ回転させていく
					bossTank->SetTurretDeg(bossTank->GetTurretDeg() + ROLLING_DEG);
				}

				//規定フレームになったら終了
				if (taskTime == ROLLING_TIME_END)
				{
					//プレイヤーとの距離を取得
					Player* player = FindGO<Player>("player");
					Vector3 distance = player->GetPosition() - bossTank->GetPosition();

					//プレイヤーとの距離が近かったら
					if (distance.LengthSq() < ROLLING_RANGE * ROLLING_RANGE)
					{
						distance.y = 0.0f;
						distance.Normalize();

						//プレイヤーを大きく吹き飛ばす
						player->KnockDown(distance * ROLLING_KNOCKDOWN_POWER);
						SoundOneShotPlay(DAMAGE_SOUND_PATH);
					}

					return true;
				}

				return false;
			}
		);

		//ボスにこのタスクをセットする
		bossTank->SetTask(enRolling, RollingTask);
	}

	void BossTankTasks::SubmitSummonTask(BossTank* bossTank)
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
					cameraTarget.Lerp(t, player->GetCameraTarget(),bossTank->GetPosition());
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
					//毎フレーム少しずつ回転させていく
					bossTank->SetBaseDeg(bossTank->GetBaseDeg() + ROLLING_DEG);
				}

				//規定フレームになったら終了
				if (taskTime == ROLLING_TIME_END)
				{
					//TODO:距離ではなくトリガーで判定したい所
					Player* player = FindGO<Player>("player");
					Vector3 distance = player->GetPosition() - bossTank->GetPosition();

					//プレイヤーとの距離が近かったら
					if (distance.LengthSq() < ROLLING_RANGE * ROLLING_RANGE)
					{
						distance.Normalize();

						//プレイヤーを大きく吹き飛ばす
						player->KnockDown(distance * ROLLING_KNOCKDOWN_POWER);
					}
					return true;
				}

				return false;
			}
		);

		//タスクの終了時にダメージ判定をする
		ChargeTask.SetEndFunc([bossTank]()
			{
			}
		);

		//ボスにこのタスクをセットする
		bossTank->SetTask(enCharge, ChargeTask);
	}

	void BossTankTasks::SubmitRockTask(BossTank* bossTank)
	{
		EnemyTask RockTask;

		RockTask.SetStartFunc([bossTank]()
			{
				//警告エフェクトを再生
				Effect* warningEffect = NewGO<Effect>(Priority::High);
				warningEffect->Init(EFFECT_WARNING_PATH);
				warningEffect->SetPosition(bossTank->GetPosition() + EFFECT_WARNING_HEIGHT);
				warningEffect->Play();
			}
		);

		//岩を作成する関数
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
				//規定フレームになると岩を発射
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

					//3つ発射したらタスクは終了
					return true;
				}


				return false;
			}
		);

		//ボスにこのタスクをセットする
		bossTank->SetTask(enRock, RockTask);
	}

	void BossTankTasks::SubmitWaitTask(BossTank* bossTank)
	{
		EnemyTask WaitTask;

		WaitTask.SetUpdateFunc([](int taskTime)
			{
				if (taskTime == WAIT_TIME)
				{
					//待機時間分待機したらタスクは終了
					return true;
				}
				else
				{
					return false;
				}
			}
		);

		//ボスにこのタスクをセットする
		bossTank->SetTask(enWait, WaitTask);
	}

	void BossTankTasks::SubmitStepTask(BossTank* bossTank)
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

		bossTank->SetTask(enStep,StepTask);
	}

	void BossTankTasks::SubmitEventRollingTask(BossTank* bossTank)
	{
		EnemyTask EventRollingTask;

		EventRollingTask.SetUpdateFunc([bossTank](int taskTime)->bool
			{
				if (taskTime == ROLLING_TIME_START)
				{
					//回転サウンドを再生
					SoundOneShotPlay(ROLLING_SOUND_PATH);
				}

				if (taskTime >= ROLLING_TIME_START)
				{
					//毎フレーム少しずつ回転させていく
					bossTank->SetTurretDeg(bossTank->GetTurretDeg() + ROLLING_DEG);
				}

				//規定フレームになったら終了
				if (taskTime == ROLLING_TIME_END)
				{
					//プレイヤーへの向きを取得
					Player* player = FindGO<Player>("player");
					Vector3 distance = player->GetPosition() - bossTank->GetPosition();

					distance.y = 0.0f;
					distance.Normalize();

					//プレイヤーを大きく吹き飛ばす
					player->KnockDown(distance * ROLLING_KNOCKDOWN_POWER);

					return true;
				}

				return false;
			}
		);

		//ボスにこのタスクをセットする
		bossTank->SetTask(enEventRolling, EventRollingTask);
	}
}