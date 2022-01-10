#include "stdafx.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "Fade.h"

namespace
{
	const UINT TITLE_SPRITE_WIDTH = 1280;
	const UINT TITLE_SPRITE_HEIGHT = 720;

	const Vector4 FONT_COLOR_BLACK = { 0.0f,0.0f,0.0f,1.0f };

	const float FADE_RATE = 0.01f;
	const int FADE_WAITTIME = 10;
}

namespace Game
{
	TitleScene::~TitleScene()
	{
		DeleteGO(m_titleSprite);
		DeleteGO(m_titleFont);
	}

	bool TitleScene::Start()
	{
		m_titleSprite = NewGO<SpriteRender>(0, "titlesprite");
		m_titleSprite->Init("Assets/Image/TitleBG.dds", TITLE_SPRITE_WIDTH, TITLE_SPRITE_HEIGHT);

		m_titleFont = NewGO<FontRender>(1, "fontrender");
		m_titleFont->SetPosition({ -150.0f, 0.0f });
		m_titleFont->SetColor(FONT_COLOR_BLACK);
		m_titleFont->SetText(L"Press A to Start");

		return true;
	}

	void TitleScene::Update()
	{
		if (m_titleFade != nullptr && m_titleFade->GetFadeProgress() == 1.0f)
		{
			NewGO<GameScene>(0, "gamescene");
			DeleteGO(this);
		}

		if (g_pad[0]->IsTrigger(enButtonA))
		{
			m_titleFade = NewGO<Fade>(0, "fade");
			m_titleFade->SetFadeInRate(FADE_RATE);
			m_titleFade->SetWaitFrame(FADE_WAITTIME);
			m_titleFade->SetFadeOutRate(FADE_RATE);
		}
	}
}