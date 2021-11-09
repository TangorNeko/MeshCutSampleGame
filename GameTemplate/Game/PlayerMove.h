#pragma once

namespace Game
{
	class PlayerMove
	{
	public:

		void Init(Vector3& playerPosition);

		void Move(Vector3& playerPosition);

		Quaternion CalcToModelDirectionQRot();

		void TurnModelToMoveDirection(SkinModelRender* modelRender);
	private:
		CharacterController m_charaCon;							//�v���C���[�̃L�����R��
		Vector3 m_moveAmount = Vector3::Zero;					//�v���C���[�̈ړ���
		Quaternion m_toMoveDirectionRot = Quaternion::Identity;	//�v���C���[�̈ړ������ւ̃N�H�[�^�j�I��
	};
}

