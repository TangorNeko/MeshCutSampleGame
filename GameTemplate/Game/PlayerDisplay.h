#pragma once

namespace Game
{
	class PlayerDisplay
	{
	public:
		~PlayerDisplay();

		/**
		 * @brief プレイヤーの情報表示の初期化
		 * @param hp プレイヤーの体力
		*/
		void Init(int hp);

		/**
		 * @brief プレイヤーの情報表示のアップデート
		 * @param hp プレイヤーの体力
		*/
		void Update(int hp);
	private:
		FontRender* m_hpRender = nullptr;	//HPを描画するフォント
		SpriteRender* m_hpFrame = nullptr;
		SpriteRender* m_hpBar = nullptr;
		bool m_isInited = false;
		float m_alpha = 0.0f;
	};
}

