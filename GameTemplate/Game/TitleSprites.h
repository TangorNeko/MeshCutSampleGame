#pragma once

namespace Game
{
	class TitleSprites : public IGameObject
	{
	public:
		~TitleSprites();

		bool Start() override;

		void Update() override;

		/**
		 * @brief ゲームがスタートした事を通知する
		*/
		void NoticeGameStart()
		{
			m_isStarted = true;
		}
	private:
		SpriteRender* m_logoRender = nullptr;		//タイトルロゴのスプライト
		SpriteRender* m_startTextRender = nullptr;	//スタート用テキストのスプライト
		bool m_isStarted = false;					//ゲームがスタートした?
		float m_alphaProgress = 0.0f;				//透過の深度
	};
}

