#pragma once

namespace Game
{
	class PlayerAttackSound
	{
	public:
		/**
		 * @brief UŒ‚‚ğ‹óU‚è‚µ‚½‚Ì‰¹‚ğ—¬‚·
		 * @param comboNum UŒ‚‚Ì’i”
		*/
		void PlayMissSound(int comboNum);

		/**
		 * @brief UŒ‚‚ğ“–‚Ä‚½‚Ì‰¹‚ğ—¬‚·
		 * @param comboNum UŒ‚‚Ì’i”
		*/
		void PlayHitSound(int comboNum);
	};
}

