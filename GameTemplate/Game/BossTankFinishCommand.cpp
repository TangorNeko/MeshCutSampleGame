#include "stdafx.h"
#include "BossTankFinishCommand.h"
#include "Player.h"
#include "CommandInput.h"

namespace
{
	const float FINISH_COMMAND_RANGE = 800.0f;		//�g�h���̃R�}���h��\�����鋗��
}

namespace Game
{

	void BossTankFinishCommand::Execute(const Vector3& bossPosition, float bossHp)
	{
		Player* player = FindGO<Player>("player");

		//�v���C���[�Ƃ̋������擾
		Vector3 distance = player->GetPosition() - bossPosition;

		//�v���C���[�ɋ߂���Ε\��
		if (distance.LengthSq() < FINISH_COMMAND_RANGE * FINISH_COMMAND_RANGE && bossHp <= 0 && m_finishCommandInput == nullptr)
		{
			m_finishCommandInput = NewGO<CommandInput>(Priority::High);
		}
		else if(distance.LengthSq() >= FINISH_COMMAND_RANGE * FINISH_COMMAND_RANGE)
		{
			//������Δ�\��
			DeleteGO(m_finishCommandInput);
			m_finishCommandInput = nullptr;
		}

		//�R�}���h���͂���������ƃv���C���[�Ɉړ��̊J�n��ʒm����
		if (m_finishCommandInput != nullptr && m_finishCommandInput->isSuccess() == true)
		{
			player->NoticeFrontMoveStart();
		}
	}
}