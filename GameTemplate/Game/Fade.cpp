#include "stdafx.h"
#include "Fade.h"

namespace
{
	const char* SPRITE_PATH = "Assets/Image/WhiteSprite.dds";	//フェードのスプライトパス
	const UINT SPRITE_WIDTH = 1280;								//フェードのスプライトの幅
	const UINT SPRITE_HEIGHT = 720;								//フェードのスプライトの高さ
	const float FADERATE_MAX = 1.0f;							//フェードの最大値
	const float FADERATE_MIN = 0.0f;							//フェードの最小値
}

namespace Game
{
	Fade::~Fade()
	{
		//フェード用のスプライトを削除
		DeleteGO(m_fadeSprite);
	}

	bool Fade::Start()
	{
		//フェード用のスプライトを生成
		m_fadeSprite = NewGO<SpriteRender>(Priority::Low);
		m_fadeSprite->Init(SPRITE_PATH, SPRITE_WIDTH, SPRITE_HEIGHT);
		m_fadeSprite->SetMulColor(m_mulColor);
		return true;
	}

	void Fade::Update()
	{
		//フェードの進行度で分岐
		switch (m_fadeState)
		{
		case enFadeIn://フェードイン
			FadeIn();
			break;
		case enWait://待機
			Wait();
			break;
		case enFadeOut://フェードアウト
			FadeOut();
			break;
		}

		//透明度をセット
		m_fadeSprite->SetMulColor(m_mulColor);
	}


	void Fade::FadeIn()
	{
		//透明度を少しずつ下げていく
		m_mulColor.w += m_fadeInRate;

		//透明度が1以上なら
		if (m_mulColor.w >= 1.0f)
		{
			//1にする
			m_mulColor.w = 1.0f;

			//フェードインが終了したので待機状態へ
			m_fadeState = enWait;
		}
	}

	void Fade::Wait()
	{
		//指定フレーム待機
		m_currentWaitFrame++;

		//待機フレームを超えれば
		if (m_currentWaitFrame >= m_waitFrame)
		{
			//フェードアウトへ移行
			m_fadeState = enFadeOut;
		}
	}

	void Fade::FadeOut()
	{
		//透明度を少しずつ上げていく
		m_mulColor.w -= m_fadeOutRate;

		//透明度が0以下になれば
		if (m_mulColor.w <= 0.0f)
		{
			//0にする
			m_mulColor.w = 0.0f;

			//フェードアウトが終了したのでインスタンスを削除
			DeleteGO(this);
		}
	}
}
