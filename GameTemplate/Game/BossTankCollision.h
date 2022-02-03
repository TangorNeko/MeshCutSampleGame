#pragma once

namespace Game
{
	class BossTankCollision
	{
	public:
		/**
		 * @brief 当たり判定の初期化
		 * @param position ボスの座標
		 * @param qRot ボスの回転
		*/
		void Init(const Vector3& position,const Quaternion qRot);

		/**
		 * @brief 当たり判定の更新
		 * @param position ボスの座標
		 * @param qRot ボスの回転
		*/
		void Update(const Vector3& position, const Quaternion qRot);
	private:
		BoxCollider m_boxCollider;	//ボックスの当たり判定
		RigidBody m_rigidBody;		//剛体
	};
}

