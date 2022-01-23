#pragma once

namespace Game
{
	class PlayerAttackSound
	{
	public:
		/**
		 * @brief �U������U�肵�����̉��𗬂�
		 * @param comboNum �U���̒i��
		*/
		void PlayMissSound(int comboNum);

		/**
		 * @brief �U���𓖂Ă����̉��𗬂�
		 * @param comboNum �U���̒i��
		*/
		void PlayHitSound(int comboNum);
	};
}

