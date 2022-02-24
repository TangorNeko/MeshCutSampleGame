#pragma once
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerAttack
	{
	public:
		/**
		 * @brief �v���C���[�̍U�������̍X�V
		 * @param playerPosition �v���C���[�̍��W
		 * @param animParam �v���C���[�̃A�j���[�V�����p�����[�^�[
		 * @param toMoveRot �ړ������ւ̃N�H�[�^�j�I��
		*/
		void Update(const Vector3& playerPosition,PlayerAnimationParam& animParam,const Quaternion& toMoveRot);
	private:
		int m_attackTime = 0;			//�U������
		int m_comboNum = 0;				//�U���i��(0:�U�����Ă��Ȃ��@1:�R���{1�@2:�R���{2�@3:�R���{3)
	};
}

