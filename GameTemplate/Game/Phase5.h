#pragma once
#include "IPhase.h"

namespace Game
{
	class Phase5 : public IPhase
	{
		bool PhaseUpdate() override;

		/**
		 * @brief フェードを開始させる
		*/
		void StartFade();
	};
}

