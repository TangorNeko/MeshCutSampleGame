#pragma once
#include "IPhase.h"

namespace Game
{
	class Phase5 : public IPhase
	{
		bool PhaseUpdate() override;

		/**
		 * @brief �t�F�[�h���J�n������
		*/
		void StartFade();
	};
}

