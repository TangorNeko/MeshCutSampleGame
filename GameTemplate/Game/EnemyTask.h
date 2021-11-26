#pragma once

namespace Game
{
	class EnemyTask
	{
	public:
		void SetStartFunc(std::function<void()> startFunc)
		{
			m_taskStartFunc = startFunc;
		}

		void SetUpdateFunc(std::function<bool(int)> updateFunc)
		{
			m_taskUpdateFunc = updateFunc;
		}

		void SetEndFunc(std::function<void()> endFunc)
		{
			m_taskEndFunc = endFunc;
		}

		void TaskStart();

		void TaskUpdate();

		void TaskEnd();

		bool Execute();

		void Terminate();
	private:
		enum TaskState
		{
			enStart,
			enUpdate,
			enEnd,
			enExit
		};
		std::function<void()> m_taskStartFunc = []() {};
		std::function<bool(int)> m_taskUpdateFunc = [](int taskTime) { return true;};
		std::function<void()> m_taskEndFunc = []() {};
		TaskState m_taskState = enStart;
		int m_taskTime = 0;
	};
}

