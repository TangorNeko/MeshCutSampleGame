#include "stdafx.h"
#include "BossTankDisplay.h"

namespace
{
	const int MAX_HP = 400;												//最大体力
	const char* SPRITE_HPFRAME_PATH = "Assets/Image/BossHpFrame.dds";	//HPバーの枠のスプライトパス
	const UINT SPRITE_HPFRAME_WIDTH = 400;								//HPバーの枠の幅
	const UINT SPRITE_HPFRAME_HEIGHT = 40;								//HPバーの枠の高さ
	const Vector2 SPRITE_HPFRAME_PIVOT = { 0.5f,0.0f };					//HPバーの枠のピボット
	const Vector3 SPRITE_HPFRAME_POSITION = { 350.0f,-300.0f,0.0f };	//HPバーの枠の座標
	const char* SPRITE_HPBAR_PATH = "Assets/Image/HpBar.dds";			//HPバーのスプライトパス
	const UINT SPRITE_HPBAR_WIDTH = 396;								//HPバーの幅
	const UINT SPRITE_HPBAR_HEIGHT = 20;								//HPバーの高さ
	const Vector2 SPRITE_HPBAR_PIVOT = { 0.0f,0.0f };					//HPバーのピボット
	const Vector3 SPRITE_HPBAR_POSITION = { 152.0f,-298.0f,0.0f };		//HPバーの座標
	const float SPRITE_SCALE_YZ = 1.0f;									//HPバーのYとZ軸の拡大率
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
		m_hpBar->SetScale({ hpBarScale,SPRITE_SCALE_YZ,SPRITE_SCALE_YZ });
	}
}