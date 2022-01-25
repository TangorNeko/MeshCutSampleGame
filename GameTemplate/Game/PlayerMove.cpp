#include "stdafx.h"
#include "PlayerMove.h"
#include "StepObject.h"
#include "BossTank.h"

namespace
{
	const float PLAYER_HEIGHT = 120.0f;
	const float PLAYER_RADIUS = 30.0f;
	const float MOVE_SPEED = 5.0f;
	const float DASH_SPEED = 10.0f;
	const EnButton BUTTON_RUN = enButtonRB2;
	const float MISSILEJUMP_SPEED = 30.0f;
	const wchar_t* LANDING_SOUND_PATH = L"Assets/sound/LandingSE.wav";
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
		switch (m_playerMoveEvent)
		{
		case enNormal:
			return NormalMove(playerPosition, animParam);
			break;
		case enMissileMove:
			return MissileMove(playerPosition, animParam);
			break;
		case enFrontMove:
			return FrontMove(playerPosition, animParam);
			break;
		case enBackHandspring:
			return BackHandspringMove(playerPosition, animParam);
			break;
		case enKnockDown:
			return KnockDownMove(playerPosition, animParam);
			break;
		default :
			return false;
		}
	}

	void PlayerMove::KnockDown(const Vector3& moveAmount)
	{
		m_playerMoveEvent = enKnockDown;
		knockDownAmount = moveAmount;
	}

	void PlayerMove::BackHandSpring(const Vector3& moveAmount)
	{
		m_playerMoveEvent = enBackHandspring;
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
			m_moveAmount *= MOVE_SPEED;
		}

		if (m_charaCon.IsOnGround() == true)
		{
			m_aerialFrame = 0;
		}
		Vector3 Down = Vector3::Down * m_aerialFrame;

		m_moveAmount += Down;
		//キャラコンに渡す。
		playerPosition = m_charaCon.Execute(m_moveAmount, 1.0f);

		m_aerialFrame++;

		//アニメーション関連　後から分離しよう

		//座標が変化していれば
		if (m_prevPosition.LengthSq() != playerPosition.LengthSq())
		{
			//走りボタンを押していればダッシュ
			if (g_pad[0]->IsPress(BUTTON_RUN))
			{
				animParam.isRunning = true;
				animParam.isWalking = false;
			}
			else
			{
				//押していなければ歩き
				animParam.isWalking = true;
				animParam.isRunning = false;
			}
		}
		else //変化していなければ歩き、走りフラグオフ
		{
			animParam.isWalking = false;
			animParam.isRunning = false;
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
		//NOTE:仮のミサイルジャンプ処理
		if (m_isMissileMove)
		{
			m_targetCount = 1;

			animParam.isJumping = true;

			//追跡するターゲットを取得　まだ3つ存在することしか想定していない]
			//追跡開始の瞬間だけQueryGOsしてもいいが、ジャンプ先が遠いと到達時には大分座標がずれていたので
			//仮でAボタンを押している間毎フレーム検索している
			QueryGOs<StepObject>("stepObject", [this](StepObject* targetObject)->bool
				{
					m_targetPos[m_targetCount] = targetObject->GetPosition();
					m_targetCount++;
					return true;
				}
			);

			//現在の座標
			m_targetPos[0] = playerPosition;

			//仮:戦車砲身横の座標
			m_targetPos[4] = { 300.0f,250.0f,-1800.0f };

			if (m_moveState == 4)
			{

				Vector3 LastPos = { 300.0f,250.0f,-1600.0f };

				Vector3 dis = LastPos - playerPosition;

				if (dis.LengthSq() < 300.0f * 300.0f)
				{
					animParam.isJumping = false;

					m_isMissileMove = false;
					return true;
				}
			}

			//現在のターゲットへのベクトルを求める
			Vector3 distance = m_targetPos[m_moveState] - playerPosition;

			//ターゲットから50以内なら到達判定
			if (distance.LengthSq() < 50.0f * 50.0f)
			{
				//次のターゲットへ
				m_moveState++;

				//到達したので次のターゲットへ移動する最初のフレーム
				m_isMoveStartFrame = true;

				//ジャンプフレームをリセット
				m_jumpFrameCount = 0;

				animParam.isJumping = false;

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
				animParam.isJumping = false;
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
					jumpMoveVector = Vector3::Up * 5.0f * m_jumpFrameCount;
				}
				else
				{
					//下移動中なら下移動ベクトルを格納
					jumpMoveVector = Vector3::Up * 5.0f * (m_distanceCount - m_jumpFrameCount);
				}
			}

			//現在のターゲットへのベクトルを正規化して大きさ1にする
			distance.Normalize();

			//移動速度を乗算
			distance *= MISSILEJUMP_SPEED;

			//上下移動分のベクトルを加算。
			//distance += jumpMoveVector;

			//キャラコンに実行させる。
			playerPosition = m_charaCon.Execute(distance, 1.0f);

			playerPosition += jumpMoveVector;
		}

		return false;
	}

	bool PlayerMove::FrontMove(Vector3& playerPosition, PlayerAnimationParam& animParam)
	{
		animParam.isJumping = true;

		if (m_isJumpStartFrame)
		{
			BossTank* bossTank = FindGO<BossTank>("bosstank");
			Vector3 targetPos = bossTank->GetFrontPosition();

			m_frontMoveAmount = targetPos - playerPosition;
			m_frontMoveAmount /= 25.0f;
			m_jumpFrameCount = 0;
			m_isJumpStartFrame = false;
		}

		if (m_jumpFrameCount < 25)
		{
			playerPosition += m_frontMoveAmount;
		}
		if (m_jumpFrameCount == 25)
		{
			animParam.isJumping = false;
			m_charaCon.SetPosition(playerPosition);

			m_isFrontMove = false;
			return true;
		}

		m_jumpFrameCount++;

		return false;
	}

	bool PlayerMove::BackHandspringMove(Vector3& playerPosition, PlayerAnimationParam& animParam)
	{
		animParam.isBackHandSpring = true;

		if (backHandspringFrame <= 40)
		{
			Vector3 gravity = { 0.0f,-10.0f,0.0f };
			playerPosition = m_charaCon.Execute(backHandSpringAmount, 1.0f);
			playerPosition = m_charaCon.Execute(gravity, 1.0f);

			Vector3 direction = backHandSpringAmount * -1.0f;
			direction.y = 0.0f;

			direction.Normalize();
			m_playerDirection = direction;
		}

		backHandspringFrame++;
		animParam.handspringTime = backHandspringFrame;

		if (backHandspringFrame == 100)
		{
			animParam.isBackHandSpring = false;
			backHandspringFrame = 0;

			m_playerMoveEvent = enNormal;
		}
		return false;
	}

	bool PlayerMove::KnockDownMove(Vector3& playerPosition, PlayerAnimationParam& animParam)
	{
		animParam.isKnockDown = true;

		if (knockDownFrame <= 40)
		{
			playerPosition = m_charaCon.Execute(knockDownAmount, 1.0f);
		}

		knockDownFrame++;
		animParam.downTime = knockDownFrame;

		if (knockDownFrame == 150)
		{
			animParam.isKnockDown = false;
			knockDownFrame = 0;

			m_playerMoveEvent = enNormal;
		}
		return false;
	}
}