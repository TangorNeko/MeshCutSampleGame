#include "stdafx.h"
#include "BossTankDisplay.h"

namespace
{
	const wchar_t* CANNONCUT_TEXT = L"Can Cut Cannon";
	const wchar_t* ALLCUT_TEXT = L"Can Cut All";
	const Vector4 SHADOWCOLOR_BLACK = { 0.0f,0.0f,0.0f,1.0f };
	const Vector2 HP_FONT_POSITION = { 350.0f,-300.0f };
	const int MAX_HP = 400;
	const char* SPRITE_HPFRAME_PATH = "Assets/Image/BossHpFrame.dds";
	const UINT SPRITE_HPFRAME_WIDTH = 400;
	const UINT SPRITE_HPFRAME_HEIGHT = 40;
	const Vector2 SPRITE_HPFRAME_PIVOT = { 0.5f,0.0f };
	const Vector3 SPRITE_HPFRAME_POSITION = { 350.0f,-300.0f,0.0f };
	const char* SPRITE_HPBAR_PATH = "Assets/Image/HpBar.dds";
	const UINT SPRITE_HPBAR_WIDTH = 396;
	const UINT SPRITE_HPBAR_HEIGHT = 20;
	const Vector2 SPRITE_HPBAR_PIVOT = { 0.0f,0.0f };
	const Vector3 SPRITE_HPBAR_POSITION = { 152.0f,-298.0f,0.0f };
}

namespace Game
{
	BossTankDisplay::~BossTankDisplay()
	{
		//HPバーの画像を削除
		DeleteGO(m_hpFrame);
		DeleteGO(m_hpBar);
	}

	void BossTankDisplay::Init(int hp)
	{
		//HPバーの枠を生成
		m_hpFrame = NewGO<SpriteRender>(Priority::High);
		m_hpFrame->Init(SPRITE_HPFRAME_PATH, SPRITE_HPFRAME_WIDTH, SPRITE_HPFRAME_HEIGHT);
		m_hpFrame->SetPivot(SPRITE_HPFRAME_PIVOT);
		m_hpFrame->SetPosition(SPRITE_HPFRAME_POSITION);

		//HPバーを生成
		m_hpBar = NewGO<SpriteRender>(Priority::Middle);
		m_hpBar->Init(SPRITE_HPBAR_PATH, SPRITE_HPBAR_WIDTH, SPRITE_HPBAR_HEIGHT);
		m_hpBar->SetPivot(SPRITE_HPBAR_PIVOT);
		m_hpBar->SetPosition(SPRITE_HPBAR_POSITION);
	}

	void BossTankDisplay::Update(int hp, const Vector3& bossPosition)
	{
		//ボスの体力に応じてHPバーのスケールを変更する
		float hpBarScale = static_cast<float>(hp) / MAX_HP;
		m_hpBar->SetScale({ hpBarScale,1.0f,1.0f });
	}
}