#pragma once

namespace Game
{
	class BossTank;

	class BossTankTasks
	{
	public:
		enum BossTankTask : int
		{
			enMissile,//ƒ~ƒTƒCƒ‹UŒ‚
			enRolling,//‰ñ“]UŒ‚
			enSummon,//G‹›¢Š«
			enCharge,//“ËiUŒ‚
			enRock,//Šâ”ò‚Î‚µ
			enWait,//‘Ò‹@
			enTaskNum
		};

		void SubmitTo(BossTank* bossTank);

		void SubmitMissileTask(BossTank* bossTank);

		void SubmitRollingTask(BossTank* bossTank);

		void SubmitSummonTask(BossTank* bossTank);

		void SubmitChargeTask(BossTank* bossTank);

		void SubmitRockTask(BossTank* bossTank);

		void SubmitWaitTask(BossTank* bossTank);
	};
}

