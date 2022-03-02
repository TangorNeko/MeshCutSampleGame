#pragma once
#include "IPhase.h"

namespace Game
{
	class Phase2 : public IPhase
	{
		void Init() override;

		void PhaseStart() override;

		bool PhaseUpdate() override;
	};
}

