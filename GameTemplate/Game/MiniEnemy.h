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
		SkinModelRender* m_baseRender = nullptr;		//モデル
		Vector3 m_position = Vector3::Zero;				//座標
		Quaternion m_baseRot = Quaternion::Identity;	//回転
		Vector3 m_moveDirection = Vector3::Zero;		//移動方向
		int m_actionFrame = 0;							//行動の経過フレーム
		bool m_isDead = false;							//死亡した?
	};
}

