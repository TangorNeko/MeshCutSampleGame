#pragma once
#include "../../ExEngine/physics/tkPhysicsGhostObject.h"

namespace Game
{
	class Player;

	class EnemyRock : public IGameObject
	{
	public:
		~EnemyRock();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* cutModel, const Vector3& cutForce) override;

		/**
		 * @brief 座標を設定
		 * @param position 
		*/
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		/**
		 * @brief 移動方向を設定
		 * @param direction 
		*/
		void SetDirection(const Vector3& direction)
		{
			m_moveDirection = direction;
			m_moveDirection.Normalize();
		}

	private:
		/**
		 * @brief プレイヤーとの当たり判定
		*/
		void PlayerHitTest();
	private:
		Player* m_targetPlayer = nullptr;			//ダメージを与えるプレイヤー
		Vector3 m_position = Vector3::Zero;			//座標
		Quaternion m_qRot = Quaternion::Identity;	//回転
		Vector3 m_moveDirection = Vector3::Front;	//移動方向
		SkinModelRender* m_rockModel = nullptr;		//岩のモデル
		CPhysicsGhostObject m_sphereTrigger;		//球状の当たり判定
		bool m_isCut = false;						//切断された?
	};
}

