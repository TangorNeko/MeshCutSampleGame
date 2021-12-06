#include "stdafx.h"
#include "PlayerMove.h"

namespace
{
	const float PLAYER_HEIGHT = 120.0f;
	const float PLAYER_RADIUS = 30.0f;
	const float MOVE_SPEED = 5.0f;
	const float DASH_SPEED = 10.0f;
	const EnButton BUTTON_RUN = enButtonRB2;
}

namespace Game
{
	void PlayerMove::Init(Vector3& playerPosition)
	{
		//キャラコンを初期化
		m_charaCon.Init(PLAYER_RADIUS, PLAYER_HEIGHT,playerPosition);
	}

	void PlayerMove::Move(Vector3& playerPosition, PlayerAnimationParam& animParam)
	{
		//TODO:超場当たり処理、KnockBackMoveとかに分離した方がいいね...
		if (isKnockDown)
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
				isKnockDown = false;
				animParam.isKnockDown = false;
				knockDownFrame = 0;
			}
			return;
		}

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

		//キャラコンに渡す。
		playerPosition = m_charaCon.Execute(m_moveAmount, 1.0f);



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
	}

	void PlayerMove::KnockDown(const Vector3& moveAmount)
	{
		isKnockDown = true;
		knockDownAmount = moveAmount;
	}

	void PlayerMove::CalcToModelDirectionQRot()
	{
		//移動方向のx,zから回転角度を取得
		float turnAngle = atan2(m_playerDirection.x, m_playerDirection.z);

		//回転角度分のクォータニオンを作成
		m_toMoveDirectionRot = Quaternion::Identity;
		m_toMoveDirectionRot.SetRotation(Vector3::AxisY, turnAngle);
	}
}