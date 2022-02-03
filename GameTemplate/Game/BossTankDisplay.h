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
		SpriteRender* m_hpFrame = nullptr;	//HPバーの枠のスプライト
		SpriteRender* m_hpBar = nullptr;	//HPバーのスプライト
	};
}