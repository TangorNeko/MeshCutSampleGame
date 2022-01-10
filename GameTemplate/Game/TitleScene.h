#pragma once

namespace Game
{
	class Fade;
	class TitleScene : public IGameObject
	{
		~TitleScene();

		bool Start() override;

		void Update() override;

	private:
		SpriteRender* m_titleSprite = nullptr;	//タイトル画面
		FontRender* m_titleFont = nullptr;		//タイトルフォント
		Fade* m_titleFade = nullptr;
	};
}

