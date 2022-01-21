#pragma once

namespace Game
{
	class GameSceneBGM : public IGameObject
	{
	public:

		~GameSceneBGM();

		bool Start() override;

		void Update() override;

		void SoundChange();

	private:

		void SoundChangeUpdate();

	private:
		SoundSource* m_playingSoundSource = nullptr;

		SoundSource* m_nextSoundSource = nullptr;

		float m_playingSoundVolume = 1.0f;

		float m_nextSoundVolume = 0.0f;

		float m_soundChangeRate = 0.01f;

		bool m_isSoundChanging = false;

		enum GameBGMPhase
		{
			enTitle,
			enBattle,
		};

		GameBGMPhase m_gameBGMPhase = enTitle;

	};
}
