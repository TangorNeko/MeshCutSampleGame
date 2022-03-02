#pragma once
#include "IPhase.h"

namespace Game
{
	class Player;

	class Phase0 : public IPhase
	{
		void Init() override;

		bool PhaseUpdate() override;

	private:
		Player* m_player = nullptr;		//プレイヤーのインスタンス
	};
}
