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
		int m_attackTime = 0;			//�U������
		int m_comboNum = 0;				//�U���i��(0:�U�����Ă��Ȃ��@1:�R���{1�@2:�R���{2�@3:�R���{3)
	};
}

