#include "stdafx.h"
#include "PlayerMove.h"
#include "StepObject.h"
#include "BossTank.h"

namespace
{
	const float PLAYER_HEIGHT = 120.0f;										//プレイヤーの当たり判定のカプセルの高さ
	const float PLAYER_RADIUS = 30.0f;										//プレイヤーの当たり判定のカプセルの半径
	const float WALK_SPEED = 5.0f;											//プレイヤーの歩き速度
	const float DASH_SPEED = 10.0f;											//プレイヤーの走り速度
	const EnButton BUTTON_RUN = enButtonRB2;								//ダッシュボタン
	const float MISSILEJUMP_SPEED = 30.0f;									//ミサイル移動の移動速度
	const wchar_t* LANDING_SOUND_PATH = L"Assets/sound/LandingSE.wav";		//着地時の効果音パス
	const Vector3 TANK_RIGHT_POSITION = { 300.0f,250.0f,-1800.0f };			//戦車の右のスペースの座標
	const Vector3 MISSILEMOVE_END_POSITION = { 300.0f,250.0f,-1600.0f };	//ミサイル移動の終了座標
	const float MISSILEMOVE_END_DISTANCE = 300.0f;							//ミサイル移動の終了距離
	const float MISSILEMOVE_REACH_DISTANCE = 50.0f;							//ミサイル移動の到達距離
	const float MISSILEMOVE_VERTICAL_SPEED = 5.0f;							//ミサイル移動の垂直方向の移動速度
	const float FRONTMOVE_DIVIDENUM = 25.0f;								//ボス正面への移動の除算値(移動距離をフレーム数で割るため)
	const float FRONTMOVE_END = 25;											//ボス正面への移動の終了フレーム
	const Vector3 BACKHANDSPRING_GRAVITY = { 0.0f,-10.0f,0.0f };			//後転移動の重力
	const float BACKHANDSPRING_MOVE = 40;									//後転移動の移動フレーム
	const float BACKHANDSPRING_END = 100;									//後転移動の終了フレーム
	const float KNOCKDOWN_MOVE = 40;										//吹き飛ばしの移動フレーム
	const float KNOCKDOWN_END = 150;										//吹き飛ばしの終了フレーム
}

namespace Game
{
	void PlayerMove::Init(Vector3& playerPosition)
	{
		//キャラコンを初期化
		m_charaCon.Init(PLAYER_RADIUS, PLAYER_HEIGHT,playerPosition);
	}

	bool PlayerMove::Move(Vector3& playerPosition, PlayerAnimationParam& animParam)
	{
		//プレイヤーの移動状態によって分岐
		switch (m_playerMoveEvent)
		{
		case enNormal://通常移動
			return NormalMove(playerPosition, animParam);
			break;
		case enMissileMove://ミサイル移動
			return MissileMove(playerPosition, animParam);
			break;
		case enFrontMove://ボスの正面への移動
			return FrontMove(playerPosition, animParam);
			break;
		case enBackHandspring://後転
			return BackHandspringMove(playerPosition, animParam);
			break;
		case enKnockDown://吹き飛ばし
			return KnockDownMove(playerPosition, animParam);
			break;
		case enDead://死亡
			return DeadMove();
		default :
			return false;
		}
	}

	void PlayerMove::KnockDown(const Vector3& moveAmount)
	{
		//移動状態を吹き飛ばし状態に
		m_playerMoveEvent = enKnockDown;

		//移動量をセット
		m_knockDownAmount = moveAmount;
	}

	void PlayerMove::BackHandSpring(const Vector3& moveAmount)
	{
		//移動状態を後転状態に
		m_playerMoveEvent = enBackHandspring;
		
		//移動量をセット
		backHandSpringAmount = moveAmount;
	}

	void PlayerMove::CalcToModelDirectionQRot()
	{
		//移動方向のx,zから回転角度を取得
		float turnAngle = atan2(m_playerDirection.x, m_playerDirection.z);

		//回転角度分のクォータニオンを作成
		m_toMoveDirectionRot = Quaternion::Identity;
		m_toMoveDirectionRot.SetRotation(Vector3::AxisY, turnAngle);
	}


	//TODO:クラス分離
	bool PlayerMove::NormalMove(Vector3& playerPosition, PlayerAnimationParam& animParam)
	{
		//各軸の入力を取得
		float rightMoveAmount = g_pad[0]->GetLStickXF();
		float forwardMoveAmount = g_pad[0]->GetLStickYF();

		//カメラの前方向を取得
		Vector3 cameraFront = g_camera3D->GetForward();
		cameraFront.y = 0.0f;
		cameraFront.Normalize();

		//カメラの右方向を取得
		Vector3 cameraRight = g_camera3D->GetRight();
		cameraRight.y = 0.0f;
		cameraRight.Normalize();


		//移動方向はカメラの前方向 * Y方向への入力量 + カメラの右方向 * X方向への入力量
		m_moveAmount = cameraFront * forwardMoveAmount + cameraRight * rightMoveAmount;

		//移動方向を正規化
		m_moveAmount.Normalize();

		//移動量が最小値以上(移動量に変化があった)の時のみ向きを格納
		if (m_moveAmount.LengthSq() > FLT_EPSILON)
		{
			m_playerDirection = m_moveAmount;
		}

		//走りボタンを押していれば走る
		if (g_pad[0]->IsPress(BUTTON_RUN))
		{
			m_moveAmount *= DASH_SPEED;
		}
		else
		{
			m_moveAmount *= WALK_SPEED;
		}

		//地面にいたら空中にいたフレームをリセット
		if (m_charaCon.IsOnGround() == true)
		{
			m_aerialFrame = 0;
		}

		//プレイヤーにかかる重力
		Vector3 Down = Vector3::Down * m_aerialFrame;

		m_moveAmount += Down;
		//キャラコンに渡す。
		playerPosition = m_charaCon.Execute(m_moveAmount, 1.0f);

		m_aerialFrame++;



		//アニメーション関連
		//座標が変化していれば
		if(m_moveAmount.LengthSq() > FLT_EPSILON)
		{
			//走りボタンを押していればダッシュ
			if (g_pad[0]->IsPress(BUTTON_RUN))
			{
				animParam.playerState = PlayerAnimationParam::enRunning;
			}
			else
			{
				//押していなければ歩き
				animParam.playerState = PlayerAnimationParam::enWalk;
			}
		}
		else //変化していなければ待機
		{
			animParam.playerState = PlayerAnimationParam::enIdle;
		}

		//今フレームの座標を格納
		m_prevPosition = playerPosition;

		return false;
	}

	bool PlayerMove::MissileMove(Vector3& playerPosition, PlayerAnimationParam& animParam)
	{
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			m_isMissileMove = true;
			//ボスの方に向かせる
			SetPlayerDirection(Vector3::Back);
		}

		//NOTE:ミサイルジャンプ処理
		if (m_isMissileMove)
		{
			m_targetCount = 1;

			animParam.playerState = PlayerAnimationParam::enJumping;

			//追跡するターゲットを取得
			//追跡開始の瞬間だけQueryGOsしてもいいが、ジャンプ先が遠いと到達時には大分座標がずれていたので
			//毎フレーム検索している
			QueryGOs<StepObject>("stepObject", [this](StepObject* targetObject)->bool
				{
					m_targetPos[m_targetCount] = targetObject->GetPosition();
					m_targetCount++;
					return true;
				}
			);

			//現在の座標
			m_targetPos[0] = playerPosition;

			//戦車砲身横の座標
			m_targetPos[4] = TANK_RIGHT_POSITION;

			//最後のジャンプなら
			if (m_moveState == 4)
			{
				//終了座標を取得
				Vector3 LastPos = MISSILEMOVE_END_POSITION;

				//終了座標への距離を計算
				Vector3 dis = LastPos - playerPosition;

				//終了座標の近くに到達したら
				if (dis.LengthSq() < MISSILEMOVE_END_DISTANCE * MISSILEMOVE_END_DISTANCE)
				{
					//待機アニメーションに変更
					animParam.playerState = PlayerAnimationParam::enIdle;

					//ミサイル移動の終了
					m_isMissileMove = false;
					return true;
				}
			}

			//現在のターゲットへのベクトルを求める
			Vector3 distance = m_targetPos[m_moveState] - playerPosition;

			//ターゲットから近ければ到達判定
			if (distance.LengthSq() < MISSILEMOVE_REACH_DISTANCE * MISSILEMOVE_REACH_DISTANCE)
			{
				//次のターゲットへ
				m_moveState++;

				//到達したので次のターゲットへ移動する最初のフレーム
				m_isMoveStartFrame = true;

				//ジャンプフレームをリセット
				m_jumpFrameCount = 0;

				//待機アニメーションにする(一旦待機アニメーションを挟む事で再度ジャンプアニメーションが再生される)
				animParam.playerState = PlayerAnimationParam::enIdle;

				//着地サウンドを再生
				SoundOneShotPlay(LANDING_SOUND_PATH);
			}

			//次のターゲットに移動する最初のフレームなら
			if (m_isMoveStartFrame == true)
			{
				//現在のターゲット位置から次のターゲット位置への距離を測定
				Vector3 distanceBetweenTargets = m_targetPos[m_moveState - 1] - m_targetPos[m_moveState];
				distanceBetweenTargets.y = 0.0f;
				//距離を移動速度で割って移動にかかるフレームを計算
				m_distanceCount = distanceBetweenTargets.Length() / MISSILEJUMP_SPEED;
				m_isMoveStartFrame = false;

				animParam.playerState = PlayerAnimationParam::enIdle;
			}

			//ジャンプから何フレーム目からのカウントをインクリメント
			m_jumpFrameCount++;

			//移動にかかるフレームの半分までは上移動、過ぎると下移動
			if (m_jumpFrameCount >= m_distanceCount / 2)
			{
				m_isMovingUp = false;
			}
			else
			{
				m_isMovingUp = true;
			}

			//上下移動用のベクトル
			Vector3 jumpMoveVector = Vector3::Zero;

			if (m_jumpFrameCount <= m_distanceCount)
			{
				if (m_isMovingUp)
				{
					//上移動中なら上移動ベクトルを格納
					jumpMoveVector = Vector3::Up * MISSILEMOVE_VERTICAL_SPEED * m_jumpFrameCount;
				}
				else
				{
					//下移動中なら下移動ベクトルを格納
					jumpMoveVector = Vector3::Up * MISSILEMOVE_VERTICAL_SPEED * (m_distanceCount - m_jumpFrameCount);
				}
			}

			//現在のターゲットへのベクトルを正規化して大きさ1にする
			distance.Normalize();

			//移動速度を乗算
			distance *= MISSILEJUMP_SPEED;

			//キャラコンに実行させる。
			playerPosition = m_charaCon.Execute(distance, 1.0f);

			playerPosition += jumpMoveVector;
		}

		return false;
	}

	bool PlayerMove::FrontMove(Vector3& playerPosition, PlayerAnimationParam& animParam)
	{
		//ジャンプアニメーションにする
		animParam.playerState = PlayerAnimationParam::enJumping;

		//ジャンプ開始フレームなら
		if (m_isJumpStartFrame)
		{
			//ボスの正面の座標を取得
			BossTank* bossTank = FindGO<BossTank>("bosstank");
			Vector3 targetPos = bossTank->GetFrontPosition();

			//ボス正面に移動する際の1フレームの移動量を計算する
			m_frontMoveAmount = targetPos - playerPosition;
			m_frontMoveAmount /= FRONTMOVE_DIVIDENUM;
			m_jumpFrameCount = 0;
			m_isJumpStartFrame = false;
		}

		//最初に計算した移動量分ﾌﾟﾚｲﾔｰを移動させる
		if (m_jumpFrameCount < FRONTMOVE_END)
		{
			playerPosition += m_frontMoveAmount;
		}

		//最終フレームなら
		if (m_jumpFrameCount == FRONTMOVE_END)
		{
			//待機アニメーションに変更
			animParam.playerState = PlayerAnimationParam::enIdle;

			//キャラコンの座標を設定
			m_charaCon.SetPosition(playerPosition);

			//正面移動の終了
			m_isFrontMove = false;
			return true;
		}

		//移動フレームを加算
		m_jumpFrameCount++;

		return false;
	}

	bool PlayerMove::BackHandspringMove(Vector3& playerPosition, PlayerAnimationParam& animParam)
	{
		//後転アニメーションにする
		animParam.playerState = PlayerAnimationParam::enBackHandSpring;

		//後転移動中なら
		if (backHandspringFrame <= BACKHANDSPRING_MOVE)
		{
			//設定した移動量と重力分キャラコンを実行
			Vector3 gravity = BACKHANDSPRING_GRAVITY;
			playerPosition = m_charaCon.Execute(backHandSpringAmount, 1.0f);
			playerPosition = m_charaCon.Execute(gravity, 1.0f);

			//プレイヤーの向きを計算する
			Vector3 direction = backHandSpringAmount * -1.0f;
			direction.y = 0.0f;

			direction.Normalize();
			m_playerDirection = direction;
		}

		//移動フレームを加算
		backHandspringFrame++;

		//後転の終了フレームなら
		if (backHandspringFrame == BACKHANDSPRING_END)
		{
			//待機アニメーションに戻す
			animParam.playerState = PlayerAnimationParam::enIdle;

			//経過フレームをリセット
			backHandspringFrame = 0;

			//移動状態を通常に戻す
			m_playerMoveEvent = enNormal;
		}
		return false;
	}

	bool PlayerMove::KnockDownMove(Vector3& playerPosition, PlayerAnimationParam& animParam)
	{
		//吹き飛ばしアニメーションにする
		animParam.playerState = PlayerAnimationParam::enKnockDown;

		//吹き飛ばし移動中なら
		if (m_knockDownFrame <= KNOCKDOWN_MOVE)
		{
			//キャラコンに移動量を渡す
			playerPosition = m_charaCon.Execute(m_knockDownAmount, 1.0f);
		}

		//移動フレームを加算
		m_knockDownFrame++;

		//吹き飛ばしの終了フレームなら
		if (m_knockDownFrame == KNOCKDOWN_END)
		{
			//待機アニメーションに戻す
			animParam.playerState = PlayerAnimationParam::enIdle;
			
			//経過フレームをリセット
			m_knockDownFrame = 0;

			//移動状態を通常状態に戻す
			m_playerMoveEvent = enNormal;
		}
		return false;
	}
}