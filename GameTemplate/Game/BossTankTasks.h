#pragma once

namespace Game
{
	class BossTank;

	class BossTankTasks
	{
	public:
		enum BossTankTask : int
		{
			enMissile,//�~�T�C���U��
			enRolling,//��]�U��
			enSummon,//�G������
			enCharge,//�ːi�U��
			enRock,//���΂�
			enWait,//�ҋ@
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

