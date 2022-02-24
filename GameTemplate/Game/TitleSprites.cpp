#include "stdafx.h"
#include "TitleSprites.h"

namespace
{
	const char* SPRITE_TITLELOGO_PATH = "Assets/Image/TitleLogo.dds";		//タイトルロゴのスプライトパス
	const UINT SPRITE_TITLELOGO_WIDTH = 700;								//タイトルロゴの幅
	const UINT SPRITE_TITLELOGO_HEIGHT = 200;								//タイトルロゴの高さ
	const Vector3 SPRITE_TITLELOGO_POSITION = { 200.0f,100.0f,0.0f };		//タイトルロゴの座標
	const char* SPRITE_STARTTEXT_PATH = "Assets/Image/StartText.dds";		//スタートテキストのスプライトパス
	const UINT SPRITE_STARTTEXT_WIDTH = 320;								//スタートテキストの幅
	const UINT SPRITE_STARTTEXT_HEIGHT = 64;								//スタートテキストの高さ
	const Vector3 SPRITE_STARTTEXT_POSITION = { 200.0f,-100.0f,0.0f };		//スタートテキストの座標
	const Vector4 SPRITE_MULTICOLOR_OPAQUE = { 1.0f,1.0f,1.0f,1.0f };		//スプライトの不透明カラー
	const Vector4 SPRITE_MULTICOLOR_TRANSPARENT = { 1.0f,1.0f,1.0f,0.0f };	//スプライトの透過カラー
	const float ALPHA_INCREASE_RATE = 0.05f;								//スプライトの透過進行度

}

namespace Game
{
	TitleSprites::~TitleSprites()
	{
		//タイトルロゴとスタートテキストを削除
		DeleteGO(m_logoRender);
		DeleteGO(m_startTextRender);
	}

	bool TitleSprites::Start()
	{
		//ロゴ画像を作成
		m_logoRender = NewGO<SpriteRender>(Priority::High);
		m_logoRender->Init(SPRITE_TITLELOGO_PATH, SPRITE_TITLELOGO_WIDTH, SPRITE_TITLELOGO_HEIGHT);
		m_logoRender->SetPosition(SPRITE_TITLELOGO_POSITION);

		//スタート用テキストを作成
		m_startTextRender = NewGO<SpriteRender>(Priority::High);
		m_startTextRender->Init(SPRITE_STARTTEXT_PATH, SPRITE_STARTTEXT_WIDTH, SPRITE_STARTTEXT_HEIGHT);
		m_startTextRender->SetPosition(SPRITE_STARTTEXT_POSITION);
		return true;
	}

	void TitleSprites::Update()
	{
		//スタートしていなかったら何もしない
		if (m_isStarted == false)
		{
			return;
		}

		//透過の深度を増やす。
		m_alphaProgress += ALPHA_INCREASE_RATE;

		//乗算カラーを線形補間
		Vector4 mulColor;
		mulColor.Lerp(m_alphaProgress, SPRITE_MULTICOLOR_OPAQUE, SPRITE_MULTICOLOR_TRANSPARENT);

		//乗算カラーをセット
		m_logoRender->SetMulColor(mulColor);
		m_startTextRender->SetMulColor(mulColor);

		//透明度が0になったら削除
		if (mulColor.w <= 0.0f)
		{
			DeleteGO(this);
		}
	}
}