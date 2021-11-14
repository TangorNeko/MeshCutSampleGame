#include "stdafx.h"
#include "PlayerMove.h"

namespace
{
	const float PLAYER_HEIGHT = 10.0f;
	const float PLAYER_RADIUS = 5.0f;
	const float MOVE_SPEED = 5.0f;
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

		m_moveAmount *= MOVE_SPEED;

		//キャラコンに渡す。
		playerPosition = m_charaCon.Execute(m_moveAmount, 1.0f);



		//アニメーション関連　後から分離しよう
		
		//座標が変化していれば歩きフラオン
		if (m_prevPosition.LengthSq() != playerPosition.LengthSq())
		{
			animParam.isWalking = true;
		}
		else //変化していなければ歩きフラグオフ
		{
			animParam.isWalking = false;
		}

		//今フレームの座標を格納
		m_prevPosition = playerPosition;
	}

	Quaternion PlayerMove::CalcToModelDirectionQRot()
	{
		//移動量が最小値以上(移動量に変化があった)の時のみ計算
		if (m_moveAmount.LengthSq() < FLT_MIN)
		{
			return m_toMoveDirectionRot;
		}

		//移動方向のx,zから回転角度を取得
		float turnAngle = atan2(m_moveAmount.x, m_moveAmount.z);

		//回転角度分のクォータニオンを作成
		m_toMoveDirectionRot = Quaternion::Identity;
		m_toMoveDirectionRot.SetRotation(Vector3::AxisY, turnAngle);

		return m_toMoveDirectionRot;
	}

	void PlayerMove::TurnModelToMoveDirection(SkinModelRender* modelRender)
	{
		//移動量が限りなく少ない場合向きを変更しない
		if (m_moveAmount.LengthSq() > FLT_MIN && modelRender != nullptr)
		{
			//モデルに回転を反映させる。
			modelRender->SetRotation(CalcToModelDirectionQRot());
		}
	}
}