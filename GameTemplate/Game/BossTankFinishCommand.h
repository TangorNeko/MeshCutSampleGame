#pragma once

namespace Game
{
	class CommandInput;

	class BossTankFinishCommand
	{
	public:

		void Execute(const Vector3& bossPosition, float bossHp);

	private:
		CommandInput* m_finishCommandInput = nullptr; //トドメ用のコマンドボタン
	};
}
