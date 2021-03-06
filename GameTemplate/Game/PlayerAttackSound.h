#pragma once

namespace Game
{
	class PlayerAttackSound
	{
	public:
		/**
		 * @brief 攻撃を空振りした時の音を流す
		 * @param comboNum 攻撃の段数
		*/
		void PlayMissSound(int comboNum);

		/**
		 * @brief 攻撃を当てた時の音を流す
		 * @param comboNum 攻撃の段数
		*/
		void PlayHitSound(int comboNum);
	};
}

