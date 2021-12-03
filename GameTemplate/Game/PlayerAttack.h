#pragma once
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerAttack
	{
	public:
		~PlayerAttack();

		void Update(const Vector3& playerPosition,PlayerAnimationParam& animParam,const Quaternion& toMoveRot);

		SkinModelRender* GetModel()
		{
			return m_testHitBox;
		}

	private:
		SkinModelRender* m_testHitBox = nullptr;
		int m_attackTime = 0;			//攻撃時間
		int m_comboNum = 0;				//攻撃段数(0:攻撃していない　1:コンボ1　2:コンボ2　3:コンボ3)
	};
}

