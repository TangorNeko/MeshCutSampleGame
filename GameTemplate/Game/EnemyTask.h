#pragma once

namespace Game
{
	class EnemyTask
	{
	public:
		/**
		 * @brief タスクのスタート時に呼ばれる関数をセットする
		 * @param startFunc タスクのスタート時に呼ばれる関数
		*/
		void SetStartFunc(std::function<void()> startFunc)
		{
			m_taskStartFunc = startFunc;
		}

		/**
		 * @brief タスクのアップデート時に呼ばれる関数をセットする
		 * @param updateFunc タスクのアップデート時に呼ばれる関数
		*/
		void SetUpdateFunc(std::function<bool(int)> updateFunc)
		{
			m_taskUpdateFunc = updateFunc;
		}

		/**
		 * @brief タスクの終了時に呼ばれる関数をセットする
		 * @param endFunc タスクの終了時に呼ばれる関数
		*/
		void SetEndFunc(std::function<void()> endFunc)
		{
			m_taskEndFunc = endFunc;
		}

		/**
		 * @brief タスクスタート時の処理
		*/
		void TaskStart();

		/**
		 * @brief タスクアップデート時の処理
		*/
		void TaskUpdate();

		/**
		 * @brief タスク終了時の処理
		*/
		void TaskEnd();

		/**
		 * @brief タスクの実行
		 * @return タスクが終了した?
		*/
		bool Execute();

		/**
		 * @brief タスクの強制終了
		*/
		void Terminate();
	private:
		enum TaskState
		{
			enStart,	//タスクスタート
			enUpdate,	//タスクアップデート
			enEnd,		//タスク終了
			enExit		//タスクから抜ける
		};
		std::function<void()> m_taskStartFunc = []() {};								//タスクのスタート時に呼ばれる関数
		std::function<bool(int)> m_taskUpdateFunc = [](int taskTime) { return true;};	//タスクのアップデート時に呼ばれる関数
		std::function<void()> m_taskEndFunc = []() {};									//タスクの終了時に呼ばれる関数
		TaskState m_taskState = enStart;												//タスクの状態
		int m_taskTime = 0;																//タスクの経過フレーム
	};
}

