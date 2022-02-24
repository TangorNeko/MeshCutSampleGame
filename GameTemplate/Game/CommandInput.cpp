#include "stdafx.h"
#include "CommandInput.h"
#include "Player.h"

namespace
{
	const char* COMMAND_SPRITE_PATH = "Assets/Image/BButtonSprite.dds";
	const UINT COMMAND_SPRITE_SIDE = 100;
	const Vector4 COLOR_TRANSPARENT = {1.0f,1.0f,1.0f,0.0f};
	const Vector4 COLOR_OPAQUE = { 1.0f,1.0f,1.0f,1.0f };
	const int FADE_FRAME_MAX = 15;
	const Vector3 COMMAND_POSITION = {0.0f,-200.0f,0.0f};
	const wchar_t* APPEAR_SOUND_PATH = L"Assets/sound/ButtonAppearSE.wav";
	const wchar_t* SUCCESS_SOUND_PATH = L"Assets/sound/ButtonSuccessSE.wav";
	const float FADE_RATE_PROGRESS = 1.0f;
	const float COMMAND_SCALE_Z = 1.0f;
}

namespace Game
{
	CommandInput::~CommandInput()
	{
		DeleteGO(m_commandSprite);
	}

	bool CommandInput::Start()
	{
		m_commandSprite = NewGO<SpriteRender>(Priority::High);
		m_commandSprite->Init(COMMAND_SPRITE_PATH, COMMAND_SPRITE_SIDE, COMMAND_SPRITE_SIDE);
		m_commandSprite->SetPosition(COMMAND_POSITION);

		SoundOneShotPlay(APPEAR_SOUND_PATH);
		return true;
	}

	void CommandInput::Update()
	{
		if (g_pad[0]->IsTrigger(enButtonB) && m_isSuccess == false)
		{
			m_isSuccess = true;

			SoundOneShotPlay(SUCCESS_SOUND_PATH);
		}

		if (m_isSuccess)
		{
			FadeOut();
		}
	}

	void CommandInput::Pause()
	{
		if (g_pad[0]->IsTrigger(enButtonB) && m_isSuccess == false)
		{
			m_isSuccess = true;

			GameObjectManager::GetInstance()->SetPauseFlag(false);

			SoundOneShotPlay(SUCCESS_SOUND_PATH);

			Player* player = FindGO<Player>("player");
			player->MissileMoveSuccess();
		}
	}

	void CommandInput::FadeOut()
	{
		m_fadeFrame++;

		m_fadeFrame = min(m_fadeFrame, FADE_FRAME_MAX);

		float fadeRate = static_cast<float>(m_fadeFrame) / FADE_FRAME_MAX;

		float spriteScale = fadeRate + FADE_RATE_PROGRESS;
		m_commandSprite->SetScale({ spriteScale ,spriteScale,COMMAND_SCALE_Z });

		Vector4 mulColor;
		mulColor.Lerp(fadeRate, COLOR_OPAQUE, COLOR_TRANSPARENT);

		m_commandSprite->SetMulColor(mulColor);
	}
}
