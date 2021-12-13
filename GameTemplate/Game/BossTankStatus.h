#pragma once

namespace Game
{
	struct BossTankStatus
	{
		Vector3 position = Vector3::Zero;				//ボスの座標
		Quaternion baseRot = Quaternion::Identity;		//車体のクォータニオン
		Quaternion turretRot = Quaternion::Identity;	//砲塔のクォータニオン
		float baseDeg = 0.0f;							//車体の角度
		float turretDeg = 0.0f;							//砲塔の角度
		int hp = 1000;									//体力
		bool isCannonBreak = false;						//砲身が壊された?
		bool isTurretBreak = false;						//砲塔が壊された?
		bool isBaseBreak = false;						//車体が壊された?
		bool isSummonMinions = false;					//雑魚敵を召喚した?
	};
}

