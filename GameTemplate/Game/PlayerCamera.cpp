#include "stdafx.h"
#include "PlayerCamera.h"
#include "BackGround.h"

namespace
{
	const Vector3 PLAYER_TO_TARGET = { 0.0f,150.0f,0.0f };			//プレイヤーから注視点へのベクトル
	const float TOCAMERA_LENGTH = 500.0f;							//カメラへのベクトルの長さ
	const Vector3 CUTMODE_PLAYER_TO_TARGET = { 45.0f,160.0f,0.0f };	//切断モードのプレイヤーから注視点へのベクトル
	const float CUTMODE_TOCAMERA_LENGTH = 80.0f;					//切断モードのカメラへのベクトルの長さ
	const float CAMERA_SPEED = 2.0f;								//カメラの移動速度
	const float CAMERA_MAX_DEG_X = 45.0f;							//カメラのX軸周りの最大角度
	const float SPRINGCAMERA_SPACE = 10.0f;							//ばねカメラのスペースの長さ
	const float CAMERA_LERP_INCREASE_VALUE = 0.01f;					//開始時のカメラの線形補間の進行度
	const float CAMERA_LERPVALUE_MAX = 1.0f;						//開始時のカメラの線形補間の最大値
	const Vector3 FINISH_CAMERA_POSITION = { 0.0f,250.0f,-700.0f };	//トドメカメラの座標
	const Vector3 FINISH_CAMERA_TARGET = { 0.0f,0.0f,-1300.0f };	//トドメカメラの注視点
}

namespace Game
{
	PlayerCamera::PlayerCamera()
	{
		//ばねカメラ用にゲームの背景のインスタンスを取得
		m_backGround = FindGO<BackGround>("background");
	}

	void PlayerCamera::Update(const Vector3& playerPosition)
	{
		//補完率を増加させる
		m_lerpValue += CAMERA_LERP_INCREASE_VALUE;
		m_lerpValue = min(m_lerpValue, CAMERA_LERPVALUE_MAX);

		//カメラのターゲットはプレイヤーの位置から計算する
		m_cameraTarget = playerPosition;
		m_cameraTarget += PLAYER_TO_TARGET;

		Vector3 lerptarget = Vector3::Zero;
		lerptarget.Lerp(m_lerpValue, m_cutCameraTarget, m_cameraTarget);

		g_camera3D->SetTarget(lerptarget);

		//横方向、縦方向の入力を取得
		Quaternion qRotY = Quaternion::Identity;
		Quaternion qRotX = Quaternion::Identity;

		float degY = g_pad[0]->GetRStickXF() * CAMERA_SPEED;
		float degX = g_pad[0]->GetRStickYF() * -CAMERA_SPEED;

		//X軸カメラの角度制限
		m_totalDegX += degX;
		if (m_totalDegX <= -CAMERA_MAX_DEG_X || m_totalDegX >= CAMERA_MAX_DEG_X)
		{
			m_totalDegX -= degX;
			degX = 0;
		}

		//入力分回す
		qRotY.SetRotationDegY(degY);

		//カメラの上下移動の軸はカメラの右とする
		qRotX.SetRotationDeg(g_camera3D->GetRight(),degX);

		//カメラターゲットからカメラまでの向きに回転を適用
		qRotY.Apply(m_toCameraDirection);
		qRotX.Apply(m_toCameraDirection);

		//カメラターゲット位置にカメラまでの向き*カメラまでの長さをかけてカメラ位置を取得
		m_cameraPosition = m_cameraTarget + m_toCameraDirection * TOCAMERA_LENGTH;

		Vector3 pos = Vector3::Zero;

		Vector3 crossPoint;
		//カメラ位置への線分がステージと当たっているかチェック
		bool isHit = m_backGround->isLineHitModel(m_cameraTarget, m_cameraPosition, crossPoint);

		//ステージに当たったら
		if (isHit == true)
		{
			Vector3 toCameraDirection = m_cameraPosition - m_cameraTarget;
			toCameraDirection.Normalize();
			//そのままだと壁に埋まるので少しスペースをあける
			pos = crossPoint - toCameraDirection * SPRINGCAMERA_SPACE;
		}
		else
		{
			//カメラ位置をセット
			pos = m_cameraPosition;
		}

		//座標を線形補間する
		Vector3 lerppos = Vector3::Zero;
		lerppos.Lerp(m_lerpValue, m_cutCameraPosition, pos);
		g_camera3D->SetPosition(lerppos);
	}

	Vector3 PlayerCamera::UpdateCutMode(const Vector3& playerPosition, const Vector3& playerDirection)
	{
		//プレイヤーの現在の向きまでの回転をセット
		Quaternion toDirection = Quaternion::Identity;
		toDirection.SetRotation(Vector3::Front, playerDirection);

		//プレイヤーの向きまでの回転をターゲットへのベクトルに適用
		Vector3 toTarget = CUTMODE_PLAYER_TO_TARGET;
		toDirection.Apply(toTarget);

		//カメラのターゲットはプレイヤーの位置から計算する
		m_cutCameraTarget = playerPosition;
		m_cutCameraTarget += toTarget;

		g_camera3D->SetTarget(m_cutCameraTarget);

		//横方向、縦方向の入力を取得
		Quaternion qRotY = Quaternion::Identity;
		Quaternion qRotX = Quaternion::Identity;

		//切断モード中は左スティックでカメラ移動させる
		float inputDegY = g_pad[0]->GetLStickXF() * CAMERA_SPEED;
		float inputDegX = g_pad[0]->GetLStickYF() * -CAMERA_SPEED;

		//X軸カメラの角度制限
		m_totalDegX += inputDegX;
		if (m_totalDegX <= -CAMERA_MAX_DEG_X || m_totalDegX >= CAMERA_MAX_DEG_X)
		{
			m_totalDegX -= inputDegX;
			inputDegX = 0;
		}

		//入力分回す
		qRotY.SetRotationDegY(inputDegY);

		//カメラの上下移動の軸はカメラの右とする
		qRotX.SetRotationDeg(g_camera3D->GetRight(), inputDegX);

		//カメラターゲットからカメラまでの向きに回転を適用
		qRotY.Apply(m_toCameraDirection);
		qRotX.Apply(m_toCameraDirection);

		//カメラターゲット位置にカメラまでの向き*カメラまでの長さをかけてカメラ位置を取得
		m_cutCameraPosition = m_cutCameraTarget + m_toCameraDirection * CUTMODE_TOCAMERA_LENGTH;

		//カメラ位置をセット
		g_camera3D->SetPosition(m_cutCameraPosition);

		//カメラ移動後の新しいプレイヤーの向きを返す
		//TODO:現在の処理は切断モードに入った際プレイヤーの向きがカメラに追従するようになっているが、
		//MGR本家ではプレイヤーの向きにカメラが追従する処理になっていた。変更してもいいかも。
		Vector3 front = g_camera3D->GetForward();
		front.y = 0.0f;
		front.Normalize();
		return front;
	}

	void PlayerCamera::UpdateFinishCamera(PlayerAnimationParam& animParam)
	{
		//トドメカメラが開始されたら納刀する
		animParam.playerState = PlayerAnimationParam::enUnequip;

		//トドメカメラ用の座標と注視点を設定
		g_camera3D->SetPosition(FINISH_CAMERA_POSITION);
		g_camera3D->SetTarget(FINISH_CAMERA_TARGET);
	}
}