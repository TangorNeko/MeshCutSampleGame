#include "stdafx.h"
#include "EnemyMissileMove.h"
#include "Player.h"

namespace
{
	const float MISSILE_SPEED = 30.0f;
	const Vector3 ERROR_VECTOR3 = { 1000.0f,1000.0f,1000.0f };
	const Vector3 PLAYER_TO_CUTPOINT = { 0.0f,120.0f,0.0f };
	const int DIRECTIONLIST_MAX = 15;
}

namespace Game
{
	EnemyMissileMove::~EnemyMissileMove()
	{
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

		//最近のフレームとの平均値を取り、移動方向を決める
		CalcAvg(m_moveDirection);

		//現在の座標からプレイヤー向きにスピード分移動させる
		//TODO:切断モード中のスローのイメージ、プレイヤー以外をスローにするようなクラスを作成する
		if (g_pad[0]->IsPress(enButtonLB1))
		{
			position += m_moveDirection * MISSILE_SPEED /4;
		}
		else
		{
			position += m_moveDirection * MISSILE_SPEED;
		}
	}

	void EnemyMissileMove::CalcAvg(Vector3& direction)
	{
		//向きをリストの後ろに詰めていく
		m_pastDirectionList.push_back(direction);

		int listSize = static_cast<int>(m_pastDirectionList.size());
		//リストのサイズが最大値より多いなら
		if (listSize > DIRECTIONLIST_MAX)
		{
			//一番昔に追加された要素を削除
			m_pastDirectionList.pop_front();
			listSize--;
		}

		Vector3 directionSum = Vector3::Zero;

		//過去のフレームの移動方向を加算していく
		for (auto& pastDirection : m_pastDirectionList)
		{
			directionSum += pastDirection;
		}
		
		//過去のフレームの移動方向との平均が現フレームの移動方向になる
		direction = directionSum / listSize;
	}
}