#include "stdafx.h"
#include "AttackTutorial.h"

namespace
{
	const char* SPRITE_PATH = "Assets/Image/AttackTutorial.dds";
	const UINT SPRITE_WIDTH = 244;
	const UINT SPRITE_HEIGHT = 40;
	const Vector2 SPRITE_PIVOT = { 0.0f,0.0f };
	const Vector3 SPRITE_STARTPOSITION = { -794.0f,-260.0f,0.0f };
	const Vector3 SPRITE_POSITION = { -550.0f,-260.0f,0.0f };
	const int FRAME_MAX = 15;
	const wchar_t* APPEAR_SOUND_PATH = L"Assets/sound/TutorialAppearSE.wav";
}

namespace Game
{
	AttackTutorial::~AttackTutorial()
	{
		DeleteGO(m_spriteRender);
	}

	bool AttackTutorial::Start()
	{
		//攻撃チュートリアルのスプライトを生成
		m_spriteRender = NewGO<SpriteRender>(Priority::High);
		m_spriteRender->Init(SPRITE_PATH, SPRITE_WIDTH, SPRITE_HEIGHT);
		m_spriteRender->SetPivot(SPRITE_PIVOT);
		m_spriteRender->SetPosition(SPRITE_STARTPOSITION);

		//生成した時の音を再生
		SoundOneShotPlay(APPEAR_SOUND_PATH);

		return true;
	}

	void AttackTutorial::Update()
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
