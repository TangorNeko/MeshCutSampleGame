#pragma once
#include "IPhase.h"

namespace Game
{
	class Phase4 : public IPhase
	{
		void Init() override;

		void PhaseStart() override;

		bool PhaseUpdate() override;
	};
}

