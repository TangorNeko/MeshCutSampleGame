#pragma once

namespace Game
{
	class GameSceneBGM : public IGameObject
	{
	public:

		~GameSceneBGM();

		bool Start() override;

		void Update() override;

		/**
		 * @brief サウンドを変更する
		*/
		void SoundChange();

	private:
		/**
		 * @brief サウンド変更中の更新処理
		*/
		void SoundChangeUpdate();

	private:
		SoundSource* m_playingSoundSource = nullptr;	//現在プレイ中のサウンド
		SoundSource* m_nextSoundSource = nullptr;		//次にプレイするサウンド
		float m_playingSoundVolume = 1.0f;				//現在プレイ中のサウンドの音量
		float m_nextSoundVolume = 0.0f;					//次にプレイするサウンドの音量
		float m_soundChangeRate = 0.01f;				//音量の変更のレート
		bool m_isSoundChanging = false;					//サウンドを変更中?

		//ゲームのBGMのフェーズ
		enum GameBGMPhase
		{
			enTitle,
			enBattle,
		};

		//現在のゲームのBGMのフェーズ
		GameBGMPhase m_gameBGMPhase = enTitle;

	};
}
