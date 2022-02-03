#pragma once

namespace Game
{
	class StepObject : public IGameObject
	{
	public:
		~StepObject();

		bool Start() override;

		void Update() override;

		/**
		 * @brief 座標を設定する
		 * @param position 座標
		*/
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		/**
		 * @brief 座標を取得する
		 * @return 座標
		*/
		const Vector3& GetPosition()
		{
			return m_position;
		}

		/**
		 * @brief 移動方向を指定する
		 * @param moveDirection 
		*/
		void SetMoveDirection(const Vector3& moveDirection)
		{
			m_moveDirection = moveDirection;
			m_moveDirection.Normalize();
		}
	private:
		Quaternion m_qRot;								//モデルの回転
		Vector3 m_moveDirection = Vector3::AxisZ;		//移動方向
		Vector3 m_position =  Vector3::Zero;			//座標
		SkinModelRender* m_skinModelRender = nullptr;	//スキンモデルレンダー
		int m_lifeTime = 0;								//生存時間
		Effect* m_missileEffect = nullptr;				//ミサイルのエフェクト
	};
}

