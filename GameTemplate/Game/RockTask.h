#pragma once

namespace Game
{
	class BossTank;

	class RockTask
	{
	public:

		/**
		 * @brief �󂯎�����{�X�Ƀ^�X�N���Z�b�g����
		 * @param bossTank �^�X�N���Z�b�g����{�X
		 * @param taskNum �^�X�N�ԍ�
		*/
		void SubmitTo(BossTank* bossTank, int taskNum);
	};
}