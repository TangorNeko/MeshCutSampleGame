#include "stdafx.h"
#include "PlayerDisplay.h"

namespace
{
	const Vector4 SHADOWCOLOR_BLACK = { 0.0f,0.0f,0.0f,1.0f };
	const Vector2 HP_POSITION = { -600.0f,330.0f };
}

namespace Game
{
	PlayerDisplay::~PlayerDisplay()
	{
		//フォントレンダーを削除
		DeleteGO(m_hpRender);
	}

	void PlayerDisplay::Init(int hp)
	{
		//フォントレンダーを作成
		m_hpRender = NewGO<FontRender>(1);

		//HPを文字列に変換
		wchar_t buffer[256];
		swprintf_s(buffer, L"HP:%d", hp);

		//文字列をセット
		m_hpRender->SetText(buffer);
		
		//縁取りを設定
		m_hpRender->SetShadowFlag(true);
		m_hpRender->SetShadowColor(SHADOWCOLOR_BLACK);
	
		//座標をセット
		m_hpRender->SetPosition(HP_POSITION);
	}

	void PlayerDisplay::Update(int hp)
	{
		//HPを文字列に変換
		wchar_t buffer[256];
		swprintf_s(buffer, L"HP:%d", hp);

		//文字列を更新
		m_hpRender->SetText(buffer);
	}
}