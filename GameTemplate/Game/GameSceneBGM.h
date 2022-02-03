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
		 * @brief �T�E���h��ύX����
		*/
		void SoundChange();

	private:
		/**
		 * @brief �T�E���h�ύX���̍X�V����
		*/
		void SoundChangeUpdate();

	private:
		SoundSource* m_playingSoundSource = nullptr;	//���݃v���C���̃T�E���h
		SoundSource* m_nextSoundSource = nullptr;		//���Ƀv���C����T�E���h
		float m_playingSoundVolume = 1.0f;				//���݃v���C���̃T�E���h�̉���
		float m_nextSoundVolume = 0.0f;					//���Ƀv���C����T�E���h�̉���
		float m_soundChangeRate = 0.01f;				//���ʂ̕ύX�̃��[�g
		bool m_isSoundChanging = false;					//�T�E���h��ύX��?

		//�Q�[����BGM�̃t�F�[�Y
		enum GameBGMPhase
		{
			enTitle,
			enBattle,
		};

		//���݂̃Q�[����BGM�̃t�F�[�Y
		GameBGMPhase m_gameBGMPhase = enTitle;

	};
}
