#include "stdafx.h"
#include "EnemyMissileMove.h"
#include "Player.h"

namespace
{
	const float MISSILE_SPEED = 5.0f;
	const Vector3 ERROR_VECTOR3 = { 1000.0f,1000.0f,1000.0f };
}

namespace Game
{
	void EnemyMissileMove::FindTarget()
	{
		m_trackingPlayer = FindGO<Player>("player");
	}

	void EnemyMissileMove::Update(Vector3& position)
	{
		//移動時間をインクリメント
		m_moveTime++;

		//ミサイルの座標からプレイヤーの座標へのベクトル(正規化していない移動方向)を求める
		m_moveDirection = m_trackingPlayer->GetPosition() - position;

		//２つの座標が近すぎる場合は向きにエラー値を入れてreturn
		//NOTE:正常に処理が進んだ場合m_moveDirectionは正規化され長さ1になるため
		//エラー値として長さが大きなベクトルを格納しておく
		if (m_moveDirection.LengthSq() < FLT_MIN)
		{
			m_moveDirection = ERROR_VECTOR3;
			return;
		}

		//プレイヤーへのベクトルを正規化
		m_moveDirection.Normalize();

		//現在の座標からプレイヤー向きにスピード分移動させる
		position += m_moveDirection * MISSILE_SPEED;
	}
}