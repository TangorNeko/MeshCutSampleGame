#pragma once
#include "IPhase.h"

namespace Game
{
	class Phase3 : public IPhase
	{
		void Init() override;

		void PhaseStart() override;

		bool PhaseUpdate() override;
	};
}

