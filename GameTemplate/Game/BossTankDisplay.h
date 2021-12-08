#pragma once

namespace Game
{
	class BossTankDisplay
	{
	public:
		~BossTankDisplay();

		/**
		 * @brief 情報表示の初期化
		 * @param hp ボスの体力
		*/
		void Init(int hp);

		/**
		 * @brief 情報表示の更新
		 * @param hp ボスの体力
		 * @param bossPosition ボスの座標 
		*/
		void Update(int hp,const Vector3& bossPosition);

	private:
		FontRender* m_hpRender = nullptr;			//体力を表示するフォント
		FontRender* m_cutStateRender = nullptr;		//切断状況を表示するフォント
	};
}