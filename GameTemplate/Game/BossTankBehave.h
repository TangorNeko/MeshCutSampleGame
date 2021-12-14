#pragma once
#include "BossTankStatus.h"
#include "EnemyTask.h"
#include "BossTankTasks.h"

namespace Game
{
	class BossTank;

	class BossTankBehave
	{
	public:
		/**
		 * @brief ボスのインスタンスにタスクを登録する
		 * @param bossTank ボスのインスタンスのポインタ
		*/
		void SubmitTo(BossTank* bossTank);

		/**
		 * @brief タスクを登録する
		 * @param taskNum タスクの番号
		 * @param task 登録するタスク
		*/
		void SetTask(int taskNum, EnemyTask task)
		{
			m_tankTask[taskNum] = task;
		}

		/**
		 * @brief ボスの行動を実行
		 * @param bossTankStatus 行動分岐に使うボスのステータス 
		*/
		void Execute(BossTankStatus& bossTankStatus);

		/**
		 * @brief 実行中のタスクより優先して行われるイベントをチェックする
		 * @param bossTankStatus 行動分岐に使うボスのステータス
		*/
		void CheckEvent(BossTankStatus& bossTankStatus);

		/**
		 * @brief 次のタスクを選択する
		 * @param bossTankStatus 選択に使うボスのステータス
		*/
		void SelectTask(BossTankStatus& bossTankStatus);

		/**
		 * @brief タスクを実行する
		*/
		void ExecuteTask();

		void TerminateTask();
	private:
		EnemyTask m_tankTask[BossTankTasks::enTaskNum];		//ボスのタスクの配列(原本、ここからキューにコピーされる)
		std::queue<EnemyTask> m_taskQueue;					//ボスのタスクキュー
	};
}

