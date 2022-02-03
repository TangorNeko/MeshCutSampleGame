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
		SpriteRender* m_hpFrame = nullptr;	//HPバーのフレームのスプライト
		SpriteRender* m_hpBar = nullptr;	//HPバーのスプライト
		bool m_isInited = false;			//初期化された?
		float m_alpha = 0.0f;				//透明度
	};
}

