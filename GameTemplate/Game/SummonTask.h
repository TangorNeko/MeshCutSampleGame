#pragma once

namespace Game
{
	class BossTank;

	class SummonTask
	{
	public:

		/**
		 * @brief 受け取ったボスにタスクをセットする
		 * @param bossTank タスクをセットするボス
		 * @param taskNum タスク番号
		*/
		void SubmitTo(BossTank* bossTank, int taskNum);
	};
}

