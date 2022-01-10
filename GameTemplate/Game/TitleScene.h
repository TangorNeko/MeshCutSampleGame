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
		SpriteRender* m_titleSprite = nullptr;	//�^�C�g�����
		FontRender* m_titleFont = nullptr;		//�^�C�g���t�H���g
		Fade* m_titleFade = nullptr;
	};
}

