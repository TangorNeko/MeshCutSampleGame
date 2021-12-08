#pragma once

namespace Game
{
	class MiniEnemy : public IGameObject
	{
	public:
		~MiniEnemy();

		bool Start() override;

		void Update() override;

		void OnDivide(const SkinModelRender* skinModelRender, const Vector3& cutForce) override;

		/**
		 * @brief 座標を設定
		 * @param position 
		*/
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
	private:
		SkinModelRender* m_baseRender = nullptr;		//車体のモデル
		SkinModelRender* m_turretRender = nullptr;		//砲塔のモデル
		SkinModelRender* m_cannonRender = nullptr;		//砲身のモデル
		Vector3 m_position = Vector3::Zero;				//座標
		Quaternion m_baseRot = Quaternion::Identity;	//車体の回転
		Quaternion m_turretRot = Quaternion::Identity;	//砲塔の回転
		Vector3 m_moveDirection = Vector3::Zero;		//移動方向
		int m_actionFrame = 0;							//行動の経過フレーム
		float m_turretDeg = 0.0f;						//砲塔の角度
		bool m_isDead = false;							//死亡した?
		bool m_isBaseBreak = false;						//車体が破壊された?
		bool m_isTurretBreak = false;					//砲塔が破壊された?
		bool m_isCannonBreak = false;					//砲身が破壊された?
	};
}

