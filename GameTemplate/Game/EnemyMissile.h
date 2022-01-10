#pragma once
#include "../../ExEngine/physics/tkPhysicsGhostObject.h"
#include "EnemyMissileMove.h"
#include "EnemyMissileRotation.h"

namespace Game
{
	class EnemyMissile : public IGameObject
	{
		~EnemyMissile();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce) override;

		/**
		 * @brief プレイヤーとの当たり判定
		*/
		void PlayerHitTest();

	public:
		/**
		 * @brief 座標をセット
		 * @param position 座標
		*/
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		/**
		 * @brief 座標を取得
		 * @return 座標
		*/
		const Vector3& GetPosition()
		{
			return m_position;
		}
		
		/**
		 * @brief 回転をセット
		 * @param rot 回転
		*/
		void SetRotation(const Quaternion rot)
		{
			m_qRot = rot;
		}
	private:
		Player* m_trackingPlayer = nullptr;				//追尾するプレイヤー
		SkinModelRender* m_missileRender = nullptr;		//スキンモデルレンダー
		Vector3 m_position = Vector3::Zero;				//座標
		Quaternion m_qRot = Quaternion::Identity;		//回転
		bool m_isCut = false;							//カットされた?
		CPhysicsGhostObject m_capsuleTrigger;			//当たり判定のトリガー
		EnemyMissileMove m_missileMove;					//ミサイルの移動処理クラス
		EnemyMissileRotation m_missileRotation;			//ミサイルの回転処理クラス
		Effect* m_missileEffect = nullptr;
	};
}