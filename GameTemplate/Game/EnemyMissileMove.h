#pragma once

namespace Game
{
	class Player;

	class EnemyMissileMove
	{
	public:
		~EnemyMissileMove();

		/**
		 * @brief 追尾するプレイヤーを設定する
		 * @param targetPlayer 
		*/
		void SetTarget(const Player* targetPlayer)
		{
			m_trackingPlayer = targetPlayer;
		}

		/**
		 * @brief 移動処理のアップデート
		 * @param position 処理前のミサイルの座標
		*/
		void Update(Vector3& position);

		/**
		 * @brief 移動方向を取得
		 * @return 移動方向
		*/
		const Vector3& GetDirection()
		{
			return m_moveDirection;
		}
	private:
		/**
		 * @brief 最近のフレームとの移動方向の平均を計算する
		 * @param direction 現フレームの移動方向
		*/
		void CalcAvg(Vector3& direction);
	private:
		const Player* m_trackingPlayer = nullptr;	//追尾するプレイヤー
		Vector3 m_moveDirection = Vector3::Zero;	//移動方向
		std::list<Vector3> m_pastDirectionList;		//過去の移動方向のリスト
		int m_moveTime = 0;							//移動時間
	};
}

