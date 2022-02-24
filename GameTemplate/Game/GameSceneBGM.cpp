#include "stdafx.h"
#include "GameSceneBGM.h"

namespace
{
	const float SOUND_VOLUME_MAX = 1.0f;
	const float SOUND_VOLUME_MIN = 0.0f;
	const wchar_t* START_BGM_PATH = L"Assets/sound/TitleBGM.wav";
	const wchar_t* BATTLE_BGM_PATH = L"Assets/sound/NormalBattleBGM.wav";
	const wchar_t* BOSSBATTLE_BGM_PATH = L"Assets/sound/BossBattleBGM.wav";
}

namespace Game
{
	GameSceneBGM::~GameSceneBGM()
	{
		DeleteGO(m_playingSoundSource);
		DeleteGO(m_nextSoundSource);
	}

	bool GameSceneBGM::Start()
	{
		m_playingSoundSource = NewGO<SoundSource>(Priority::High);
		m_playingSoundSource->Init(START_BGM_PATH,SoundType::enBGM);
		m_playingSoundSource->SetVolume(m_playingSoundVolume);
		m_playingSoundSource->Play(true);
		return true;
	}

	void GameSceneBGM::Update()
	{
		SoundChangeUpdate();
	}

	void GameSceneBGM::SoundChange()
	{
		//すでに再生されているサウンドをフェードアウトさせながら、次に再生するサウンドをフェードインさせる。
		m_isSoundChanging = true;

		m_nextSoundSource = NewGO<SoundSource>(Priority::High);

		//ゲームのフェーズによって分岐
		switch (m_gameBGMPhase)
		{
		case enTitle:
			m_nextSoundSource->Init(BATTLE_BGM_PATH, SoundType::enBGM);
			m_gameBGMPhase = enBattle;
			break;
		case enBattle:
			m_nextSoundSource->Init(BOSSBATTLE_BGM_PATH, SoundType::enBGM);
			break;
		default:
			break;
		}

		m_nextSoundSource->SetVolume(m_nextSoundVolume);
		m_nextSoundSource->Play(true);
	}

	void GameSceneBGM::SoundChangeUpdate()
	{
		if (m_isSoundChanging == false)
		{
			return;
		}

		m_playingSoundVolume -= m_soundChangeRate;
		m_nextSoundVolume += m_soundChangeRate;

		if (m_nextSoundVolume >= SOUND_VOLUME_MAX)
		{
			DeleteGO(m_playingSoundSource);

			m_playingSoundSource = m_nextSoundSource;

			m_nextSoundSource = nullptr;

			m_playingSoundVolume = SOUND_VOLUME_MAX;
			m_nextSoundVolume = SOUND_VOLUME_MIN;

			m_playingSoundSource->SetVolume(m_playingSoundVolume);

			m_isSoundChanging = false;
			return;
		}

		m_playingSoundSource->SetVolume(m_playingSoundVolume);
		m_nextSoundSource->SetVolume(m_nextSoundVolume);
	}
}
