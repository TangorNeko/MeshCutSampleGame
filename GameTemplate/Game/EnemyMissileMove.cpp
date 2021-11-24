#include "stdafx.h"
#include "EnemyMissileMove.h"
#include "Player.h"

namespace
{
	const float MISSILE_SPEED = 9.0f;
	const Vector3 ERROR_VECTOR3 = { 1000.0f,1000.0f,1000.0f };
	const Vector3 PLAYER_TO_CUTPOINT = { 0.0f,120.0f,0.0f };
}

namespace Game
{
	EnemyMissileMove::EnemyMissileMove()
	{
	}

	void EnemyMissileMove::FindTarget()
	{
		m_trackingPlayer = FindGO<Player>("player");
	}

	void EnemyMissileMove::Update(Vector3& position)
	{
		//移動時間をインクリメント
		m_moveTime++;

		//プレイヤーが斬りやすいように追尾する地点の高さは切断の中心の高さにする
		Vector3 targetPos = m_trackingPlayer->GetPosition() + PLAYER_TO_CUTPOINT;

		//ミサイルの座標からターゲットの座標へのベクトル(正規化していない移動方向)を求める
		m_moveDirection = targetPos - position;

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

		CalcAvg(m_moveDirection);

		//現在の座標からプレイヤー向きにスピード分移動させる
		position += m_moveDirection * MISSILE_SPEED;
	}

	void EnemyMissileMove::CalcAvg(Vector3& direction)
	{
		//向きをリストの後ろに詰めていく
		m_pastDirectionList.push_back(direction);

		int listSize = static_cast<int>(m_pastDirectionList.size());
		//リストのサイズが15より多いなら
		if (listSize > 15)
		{
			m_pastDirectionList.pop_front();
			listSize--;
		}

		Vector3 directionSum = Vector3::Zero;
		for (auto& pastDirection : m_pastDirectionList)
		{
			directionSum += pastDirection;
		}
		
		direction = directionSum / listSize;
	}
}