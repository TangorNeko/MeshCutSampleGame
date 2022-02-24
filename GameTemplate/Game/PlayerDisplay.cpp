#include "stdafx.h"
#include "PlayerDisplay.h"

namespace
{
	const float MAX_HP = 1000.0f;									//最大体力
	const char* HP_FRAME_PATH = "Assets/Image/PlayerHpFrame.dds";	//HPバーの枠のスプライトパス
	const UINT HP_FRAME_WIDTH = 400;								//HPバーの枠の幅
	const UINT HP_FRAME_HEIGHT = 40;								//HPバーの枠の高さ
	const Vector3 HP_FRAME_POSITION = { -350.0f,275.0f,0.0f };		//HPバーの枠の座標
	const Vector2 HP_FRAME_PIVOT = { 0.5f,0.0f };					//HPバーの枠のピボット
	const char* HP_BAR_PATH = "Assets/Image/HpBar.dds";				//HPバーのスプライトパス
	const UINT HP_BAR_WIDTH = 396;									//HPバーの幅
	const UINT HP_BAR_HEIGHT = 20;									//HPバーの高さ
	const Vector3 HP_BAR_POSITION = { -548.0f,277.0f,0.0f };		//HPバーの座標
	const Vector2 HP_BAR_PIVOT = { 0.0f,0.0f };						//HPバーのピボット
	const float ALPHA_INCREASE_VALUE = 0.01f;						//HPバーの透明度の増加量
	const float ALPHA_MAX = 1.0f;									//HPバーの透明度の最大値
	const float HPBAR_SCALE_YZ = 1.0f;								//HPバーのYとZ軸の拡大率
}

namespace Game
{
	PlayerDisplay::~PlayerDisplay()
	{
		//体力の画像を削除
		DeleteGO(m_hpFrame);
		DeleteGO(m_hpBar);
	}

	void PlayerDisplay::Init(int hp)
	{
		//HPバーの枠を生成
		m_hpFrame = NewGO<SpriteRender>(Priority::High);
		m_hpFrame->Init(HP_FRAME_PATH, HP_FRAME_WIDTH, HP_FRAME_HEIGHT);
		m_hpFrame->SetPivot(HP_FRAME_PIVOT);
		m_hpFrame->SetPosition(HP_FRAME_POSITION);

		//HPバーを生成
		m_hpBar = NewGO<SpriteRender>(Priority::Middle);
		m_hpBar->Init(HP_BAR_PATH, HP_BAR_WIDTH, HP_BAR_HEIGHT);
		m_hpBar->SetPivot(HP_BAR_PIVOT);
		m_hpBar->SetPosition(HP_BAR_POSITION);

		//初期化終了
		m_isInited = true;
	}

	void PlayerDisplay::Update(int hp)
	{
		//初期化されていないなら何もしない
		if (m_isInited == false)
		{
			return;
		}

		//透明度の値を上げていく(透明度を下げていく)
		m_alpha = min(ALPHA_MAX, m_alpha + ALPHA_INCREASE_VALUE);
		Vector4 mulColor = Vector4::White;
		mulColor.w = m_alpha;

		//HPバーに透明度をセット
		m_hpFrame->SetMulColor(mulColor);
		m_hpBar->SetMulColor(mulColor);
	
		//HPに応じて拡大率をセット
		m_hpBar->SetScale({ hp / MAX_HP,HPBAR_SCALE_YZ,HPBAR_SCALE_YZ });
	}
}