#pragma once

namespace Game
{
	class BossTank;

	class BossTankTasks
	{
	public:
		enum BossTankTask : int
		{
			enMissile,//ミサイル攻撃
			enRolling,//回転攻撃
			enSummon,//雑魚召喚
			enCharge,//突進攻撃
			enRock,//岩飛ばし
			enStep,//足場ミサイル
			enEventRolling,//イベント用回転攻撃(回避不可)
			enWait,//待機
			enTaskNum
		};

		/**
		 * @brief 受け取ったボスにタスクをセットする
		 * @param bossTank タスクをセットするボス
		*/
		void SubmitTo(BossTank* bossTank);

	private:
		/**
		 * @brief ミサイルを発射するタスクを登録する
		 * @param bossTank 登録するボス
		*/
		void SubmitMissileTask(BossTank* bossTank);

		/**
		 * @brief 回転攻撃をするタスクを登録する
		 * @param bossTank 登録するボス
		*/
		void SubmitRollingTask(BossTank* bossTank);

		/**
		 * @brief 雑魚敵を召喚するタスクを登録する
		 * @param bossTank 登録するボス
		*/
		void SubmitSummonTask(BossTank* bossTank);

		/**
		 * @brief 突進攻撃をするタスクを登録する
		 * @param bossTank 登録するボス
		*/
		void SubmitChargeTask(BossTank* bossTank);

		/**
		 * @brief 岩を発射するタスクを登録する
		 * @param bossTank 登録するボス
		*/
		void SubmitRockTask(BossTank* bossTank);

		/**
		 * @brief 待機するタスクを登録する
		 * @param bossTank 登録するボス
		*/
		void SubmitWaitTask(BossTank* bossTank);

		/**
		 * @brief 足場になるミサイルを発射するタスクを登録する
		 * @param bossTank 登録するボス
		*/
		void SubmitStepTask(BossTank* bossTank);

		/**
		 * @brief イベント用の回転攻撃のタスクを登録する
		 * @param bossTank 登録するボス
		*/
		void SubmitEventRollingTask(BossTank* bossTank);
	};
}

