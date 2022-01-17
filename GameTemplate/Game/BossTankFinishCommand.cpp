#include "stdafx.h"
#include "BossTankFinishCommand.h"
#include "Player.h"
#include "CommandInput.h"

namespace
{
	const float FINISH_COMMAND_RANGE = 500.0f;
}

namespace Game
{

	void BossTankFinishCommand::Execute(const Vector3& bossPosition, float bossHp)
	{
		Player* player = FindGO<Player>("player");

		Vector3 distance = player->GetPosition() - bossPosition;

		if (distance.LengthSq() < FINISH_COMMAND_RANGE * FINISH_COMMAND_RANGE && bossHp <= 0 && m_finishCommandInput == nullptr)
		{
			m_finishCommandInput = NewGO<CommandInput>(0);
		}
		else if(distance.LengthSq() >= FINISH_COMMAND_RANGE * FINISH_COMMAND_RANGE)
		{
			DeleteGO(m_finishCommandInput);
			m_finishCommandInput = nullptr;
		}

		if (m_finishCommandInput != nullptr && m_finishCommandInput->isSuccess() == true)
		{
			player->NoticeFrontMoveStart();
		}
	}
}