#pragma once

namespace Game
{
	class BossTank;

	class EventRollingTask
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

