#include "stdafx.h"
#include "CommandInput.h"

namespace
{
	const char* COMMAND_SPRITE_PATH = "Assets/Image/BButtonSprite.dds";
	const UINT COMMAND_SPRITE_SIDE = 100;
	const Vector4 COLOR_TRANSPARENT = {1.0f,1.0f,1.0f,0.0f};
	const Vector4 COLOR_OPAQUE = { 1.0f,1.0f,1.0f,1.0f };
	const int FADE_FRAME_MAX = 15;
	const Vector3 COMMAND_POSITION = {0.0f,-200.0f,0.0f};
}

namespace Game
{
	CommandInput::~CommandInput()
	{
	}

	bool CommandInput::Start()
	{
		m_commandSprite = NewGO<SpriteRender>(5);
		m_commandSprite->Init(COMMAND_SPRITE_PATH, COMMAND_SPRITE_SIDE, COMMAND_SPRITE_SIDE);
		m_commandSprite->SetPosition(COMMAND_POSITION);
		return true;
	}

	void CommandInput::Update()
	{
		if (g_pad[0]->IsTrigger(enButtonB))
		{
			m_isSuccess = true;
		}

		if (m_isSuccess)
		{
			FadeOut();
		}
	}

	void CommandInput::FadeOut()
	{
		m_fadeFrame++;

		m_fadeFrame = min(m_fadeFrame, FADE_FRAME_MAX);

		float fadeRate = static_cast<float>(m_fadeFrame) / FADE_FRAME_MAX;

		float spriteScale = fadeRate + 1.0f;
		m_commandSprite->SetScale({ spriteScale ,spriteScale,1.0f });

		Vector4 mulColor;
		mulColor.Lerp(fadeRate, COLOR_OPAQUE, COLOR_TRANSPARENT);

		m_commandSprite->SetMulColor(mulColor);
	}
}
