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

		void TurnModelToPlayerDirection(SkinModelRender* modelRender);

		const Vector3& GetPlayerDirection()
		{
			return m_playerDirection;
		}

		void SetPlayerDirection(const Vector3& direction)
		{
			m_playerDirection = direction;
		}

		CharacterController& GetCharaCon()
		{
			return m_charaCon;
		}

		void KnockDown(const Vector3& moveAmount);
	private:
		CharacterController m_charaCon;							//�v���C���[�̃L�����R��
		Vector3 m_moveAmount = Vector3::Zero;					//�v���C���[�̈ړ���
		Vector3 m_prevPosition = Vector3::Zero;					//1�t���[���O�̃v���C���[�̍��W
		Vector3 m_playerDirection = Vector3::Front;				//�v���C���[�̌���
		Quaternion m_toMoveDirectionRot = Quaternion::Identity;	//�v���C���[�̈ړ������ւ̃N�H�[�^�j�I��

		//TODO:�ϐ��̕K�v�������؂����̂�������m_���������o�[�ϐ��Ƃ���
		bool isKnockDown = false;
		int knockDownFrame = 0;
		Vector3 knockDownAmount = Vector3::Zero;
	};
}

