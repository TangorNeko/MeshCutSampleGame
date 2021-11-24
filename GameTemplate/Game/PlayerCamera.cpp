#include "stdafx.h"
#include "PlayerCamera.h"

namespace
{
	const Vector3 PLAYER_TO_TARGET = { 0.0f,150.0f,0.0f };
	const float TOCAMERA_LENGTH = 500.0f;
	const Vector3 CUTMODE_PLAYER_TO_TARGET = { 30.0f,160.0f,0.0f };
	const float CUTMODE_TOCAMERA_LENGTH = 80.0f;
	const float CAMERA_SPEED = 2.0f;
}

namespace Game
{
	void PlayerCamera::Update(const Vector3& playerPosition)
	{
		//カメラのターゲットはプレイヤーの位置から計算する
		m_cameraTarget = playerPosition;
		m_cameraTarget += PLAYER_TO_TARGET;

		g_camera3D->SetTarget(m_cameraTarget);

		//横方向、縦方向の入力を取得
		Quaternion qRotY = Quaternion::Identity;
		Quaternion qRotX = Quaternion::Identity;

		float degY = g_pad[0]->GetRStickXF() * CAMERA_SPEED;
		float degX = g_pad[0]->GetRStickYF() * -CAMERA_SPEED;

		//入力分回す
		qRotY.SetRotationDegY(degY);

		//カメラの上下移動の軸はカメラの右とする
		qRotX.SetRotationDeg(g_camera3D->GetRight(),degX);

		//カメラターゲットからカメラまでの向きに回転を適用
		qRotY.Apply(m_toCameraDirection);
		qRotX.Apply(m_toCameraDirection);

		//カメラターゲット位置にカメラまでの向き*カメラまでの長さをかけてカメラ位置を取得
		Vector3 cameraPosition = m_cameraTarget + m_toCameraDirection * TOCAMERA_LENGTH;

		//カメラ位置をセット
		g_camera3D->SetPosition(cameraPosition);
	}

	void PlayerCamera::UpdateCutMode(const Vector3& playerPosition)
	{
		//カメラのターゲットはプレイヤーの位置から計算する
		m_cameraTarget = playerPosition;
		m_cameraTarget += CUTMODE_PLAYER_TO_TARGET;

		g_camera3D->SetTarget(m_cameraTarget);

		//カメラターゲット位置にカメラまでの向き*カメラまでの長さをかけてカメラ位置を取得
		Vector3 cameraPosition = m_cameraTarget + m_toCameraDirection * CUTMODE_TOCAMERA_LENGTH;

		//カメラ位置をセット
		g_camera3D->SetPosition(cameraPosition);
	}
}