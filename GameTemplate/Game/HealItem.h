#pragma once
namespace Game
{
	class Player;

	class HealItem : public IGameObject
	{
	public:

		~HealItem();

		bool Start() override;

		void Update() override;
		
		/**
		 * @brief 座標の設定
		 * @param position 座標
		*/
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}

		/**
		 * @brief 拾われる処理
		*/
		void PickedUp();
	private:
		Vector3 m_position = Vector3::Zero;				//座標
		Quaternion m_qRot = Quaternion::Identity;		//回転
		float m_rotateDegY = 0;							//Y軸周りの回転角度
		SkinModelRender* m_skinModelRender = nullptr;	//モデルレンダー
		Player* m_player = nullptr;						//拾われるプレイヤー
		BoxCollider m_boxCollider;						//箱の当たり判定
		RigidBody m_rigidBody;							//剛体
	};
}

