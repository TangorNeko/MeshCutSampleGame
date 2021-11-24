#pragma once
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerMove
	{
	public:

		void Init(Vector3& playerPosition);

		void Move(Vector3& playerPosition,PlayerAnimationParam& animParam);

		Quaternion CalcToModelDirectionQRot();

		void TurnModelToMoveDirection(SkinModelRender* modelRender);

		CharacterController& GetCharaCon()
		{
			return m_charaCon;
		}
	private:
		CharacterController m_charaCon;							//�v���C���[�̃L�����R��
		Vector3 m_moveAmount = Vector3::Zero;					//�v���C���[�̈ړ���
		Vector3 m_prevPosition = Vector3::Zero;					//1�t���[���O�̃v���C���[�̍��W
		Quaternion m_toMoveDirectionRot = Quaternion::Identity;	//�v���C���[�̈ړ������ւ̃N�H�[�^�j�I��
	};
}

