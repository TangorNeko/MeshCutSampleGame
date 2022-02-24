#include "stdafx.h"
#include "GameSceneBGM.h"

namespace
{
	const float SOUND_VOLUME_MAX = 1.0f;									//BGMの最大音量
	const float SOUND_VOLUME_MIN = 0.0f;									//BGMの最小音量
	const wchar_t* START_BGM_PATH = L"Assets/sound/TitleBGM.wav";			//タイトルのBGMパス
	const wchar_t* BATTLE_BGM_PATH = L"Assets/sound/NormalBattleBGM.wav";	//通常バトルのBGMパス
	const wchar_t* BOSSBATTLE_BGM_PATH = L"Assets/sound/BossBattleBGM.wav";	//ボスバトルのBGMパス
}

namespace Game
{
	GameSceneBGM::~GameSceneBGM()
	{
		//再生中のBGMと次に再生予定のBGMを削除
		DeleteGO(m_playingSoundSource);
		DeleteGO(m_nextSoundSource);
	}

	bool GameSceneBGM::Start()
	{
		//タイトルBGMを作成
		m_playingSoundSource = NewGO<SoundSource>(Priority::High);
		m_playingSoundSource->Init(START_BGM_PATH,SoundType::enBGM);
		m_playingSoundSource->SetVolume(m_playingSoundVolume);
		m_playingSoundSource->Play(true);
		return true;
	}

	void GameSceneBGM::Update()
	{
		//BGM変更の処理
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
		case enTitle://タイトルBGMなら
			//次に再生するのはバトルBGM
			m_nextSoundSource->Init(BATTLE_BGM_PATH, SoundType::enBGM);
			m_gameBGMPhase = enBattle;
			break;
		case enBattle://通常バトルBGMなら
			//次に再生するのはボスバトルBGM
			m_nextSoundSource->Init(BOSSBATTLE_BGM_PATH, SoundType::enBGM);
			break;
		default:
			break;
		}

		//次に再生するBGMのボリュームをセット
		m_nextSoundSource->SetVolume(m_nextSoundVolume);
		m_nextSoundSource->Play(true);
	}

	void GameSceneBGM::SoundChangeUpdate()
	{
		//BGM変更中でないなら何もしない
		if (m_isSoundChanging == false)
		{
			return;
		}

		//現在再生中のBGMのボリュームを下げていく
		m_playingSoundVolume -= m_soundChangeRate;
		//次に再生するBGMのボリュームを上げていく
		m_nextSoundVolume += m_soundChangeRate;

		//次に再生するBGMのボリュームが最大になったら
		if (m_nextSoundVolume >= SOUND_VOLUME_MAX)
		{
			//プレイ中だったBGMの音量は0なので削除する
			DeleteGO(m_playingSoundSource);

			//ポインタを再生中のBGMに付け替える
			m_playingSoundSource = m_nextSoundSource;

			m_nextSoundSource = nullptr;

			//付け替えたので再生中のBGMのボリュームを最大に
			m_playingSoundVolume = SOUND_VOLUME_MAX;

			//次に再生するBGMのボリュームは最小に
			m_nextSoundVolume = SOUND_VOLUME_MIN;

			//再生中のBGMのボリュームをセット
			m_playingSoundSource->SetVolume(m_playingSoundVolume);

			//変更処理を終了
			m_isSoundChanging = false;
			return;
		}

		//変更処理中なら再生中BGMと次に再生するBGMの音量をセット
		m_playingSoundSource->SetVolume(m_playingSoundVolume);
		m_nextSoundSource->SetVolume(m_nextSoundVolume);
	}
}
