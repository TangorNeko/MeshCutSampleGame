#include "stdafx.h"
#include "DashTutorial.h"

namespace
{
	const char* SPRITE_PATH = "Assets/Image/DashTutorial.dds";
	const UINT SPRITE_WIDTH = 180;
	const UINT SPRITE_HEIGHT = 40;
	const Vector2 SPRITE_PIVOT = { 0.0f,0.0f };
	const Vector3 SPRITE_STARTPOSITION = { -730.0f,-300.0f,0.0f };
	const Vector3 SPRITE_POSITION = { -550.0f,-300.0f,0.0f };
	const int FRAME_MAX = 15;
}

namespace Game
{
	DashTutorial::~DashTutorial()
	{
		DeleteGO(m_spriteRender);
	}

	bool DashTutorial::Start()
	{
		m_spriteRender = NewGO<SpriteRender>(5);
		m_spriteRender->Init(SPRITE_PATH, SPRITE_WIDTH, SPRITE_HEIGHT);
		m_spriteRender->SetPivot(SPRITE_PIVOT);
		m_spriteRender->SetPosition(SPRITE_POSITION);

		return true;
	}

	void DashTutorial::Update()
	{
		m_frame++;

		m_frame = min(m_frame, FRAME_MAX);

		float rate = static_cast<float>(m_frame) / FRAME_MAX;
		Vector3 position;
		position.Lerp(rate, SPRITE_STARTPOSITION, SPRITE_POSITION);
		m_spriteRender->SetPosition(position);
	}
}