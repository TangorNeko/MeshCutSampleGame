#pragma once

namespace Game
{
	class CommandInput;

	class BossTankFinishCommand
	{
	public:

		/**
		 * @brief トドメコマンドの表示処理を実行
		 * @param bossPosition ボスの座標
		 * @param bossHp ボスの体力
		*/
		void Execute(const Vector3& bossPosition, float bossHp);

	private:
		CommandInput* m_finishCommandInput = nullptr; //トドメ用のコマンドボタン
	};
}
