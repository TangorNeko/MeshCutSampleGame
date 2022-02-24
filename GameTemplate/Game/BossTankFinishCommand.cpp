#include "stdafx.h"
#include "BossTankFinishCommand.h"
#include "Player.h"
#include "CommandInput.h"

namespace
{
	const float FINISH_COMMAND_RANGE = 800.0f;		//トドメのコマンドを表示する距離
}

namespace Game
{

	void BossTankFinishCommand::Execute(const Vector3& bossPosition, float bossHp)
	{
		Player* player = FindGO<Player>("player");

		//プレイヤーとの距離を取得
		Vector3 distance = player->GetPosition() - bossPosition;

		//プレイヤーに近ければ表示
		if (distance.LengthSq() < FINISH_COMMAND_RANGE * FINISH_COMMAND_RANGE && bossHp <= 0 && m_finishCommandInput == nullptr)
		{
			m_finishCommandInput = NewGO<CommandInput>(Priority::High);
		}
		else if(distance.LengthSq() >= FINISH_COMMAND_RANGE * FINISH_COMMAND_RANGE)
		{
			//遠ければ非表示
			DeleteGO(m_finishCommandInput);
			m_finishCommandInput = nullptr;
		}

		//コマンド入力が成功するとプレイヤーに移動の開始を通知する
		if (m_finishCommandInput != nullptr && m_finishCommandInput->isSuccess() == true)
		{
			player->NoticeFrontMoveStart();
		}
	}
}