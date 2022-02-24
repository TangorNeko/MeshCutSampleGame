#include "stdafx.h"
#include "BossTankCollision.h"

namespace Game
{
	const Vector3 TANK_COLLISION_SIZE = { 400.0f,175.0f,800.0f };	//戦車の当たり判定の大きさ
	const float TANK_COLLISION_MASS = 100.0f;						//戦車の当たり判定の質量
	const float TANK_COLLISION_FRICTION = 10.0f;					//戦車の当たり判定の摩擦力
	const Vector3 TANK_COLLISION_LINEARFACTOR = Vector3::Zero;		//戦車の当たり判定の移動可能な軸
}

namespace Game
{
	void BossTankCollision::Init(const Vector3& position, const Quaternion qRot)
	{
		//剛体を作成
		m_boxCollider.Create(TANK_COLLISION_SIZE);
		RigidBodyInitData rbInitData;
		rbInitData.mass = TANK_COLLISION_MASS;
		rbInitData.collider = &m_boxCollider;
		rbInitData.pos = position;
		rbInitData.rot = qRot;

		//剛体の初期化
		m_rigidBody.Init(rbInitData);
		m_rigidBody.SetFriction(TANK_COLLISION_FRICTION);
		m_rigidBody.SetLinearFactor(TANK_COLLISION_LINEARFACTOR);
	}

	void BossTankCollision::Update(const Vector3& position, const Quaternion qRot)
	{
		//剛体の座標と回転をセット
		m_rigidBody.SetPositionAndRotation(position, qRot);
	}
}