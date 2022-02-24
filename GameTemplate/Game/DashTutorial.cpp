#include "stdafx.h"
#include "DashTutorial.h"

namespace
{
	const char* SPRITE_PATH = "Assets/Image/DashTutorial.dds";				//ダッシュチュートリアルのスプライトパス
	const UINT SPRITE_WIDTH = 180;											//ダッシュチュートリアルの幅
	const UINT SPRITE_HEIGHT = 40;											//ダッシュチュートリアルの高さ
	const Vector2 SPRITE_PIVOT = { 0.0f,0.0f };								//ダッシュチュートリアルのピボット
	const Vector3 SPRITE_STARTPOSITION = { -730.0f,-300.0f,0.0f };			//ダッシュチュートリアルの初期座標
	const Vector3 SPRITE_POSITION = { -550.0f,-300.0f,0.0f };				//ダッシュチュートリアルの座標
	const int FRAME_MAX = 15;												//ダッシュチュートリアルの最大移動フレーム
	const wchar_t* APPEAR_SOUND_PATH = L"Assets/sound/TutorialAppearSE.wav";//ダッシュチュートリアルの出現効果音パス
}

namespace Game
{
	DashTutorial::~DashTutorial()
	{
		//ダッシュチュートリアルのスプライトを削除
		DeleteGO(m_spriteRender);
	}

	bool DashTutorial::Start()
	{
		//ダッシュチュートリアルのスプライトを生成
		m_spriteRender = NewGO<SpriteRender>(Priority::High);
		m_spriteRender->Init(SPRITE_PATH, SPRITE_WIDTH, SPRITE_HEIGHT);
		m_spriteRender->SetPivot(SPRITE_PIVOT);
		m_spriteRender->SetPosition(SPRITE_POSITION);

		//生成した時の音を再生
		SoundOneShotPlay(APPEAR_SOUND_PATH);

		return true;
	}

	void DashTutorial::Update()
	{
		//少しずつ横からスプライトをポップアップさせる
		m_popUpProgress++;

		m_popUpProgress = min(m_popUpProgress, FRAME_MAX);

		float rate = static_cast<float>(m_popUpProgress) / FRAME_MAX;
		Vector3 position;
		position.Lerp(rate, SPRITE_STARTPOSITION, SPRITE_POSITION);
		m_spriteRender->SetPosition(position);
	}
}
