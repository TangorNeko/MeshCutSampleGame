#include "stdafx.h"
#include "GameSceneBGM.h"

namespace
{
	const float SOUND_VOLUME_MAX = 1.0f;									//BGM�̍ő剹��
	const float SOUND_VOLUME_MIN = 0.0f;									//BGM�̍ŏ�����
	const wchar_t* START_BGM_PATH = L"Assets/sound/TitleBGM.wav";			//�^�C�g����BGM�p�X
	const wchar_t* BATTLE_BGM_PATH = L"Assets/sound/NormalBattleBGM.wav";	//�ʏ�o�g����BGM�p�X
	const wchar_t* BOSSBATTLE_BGM_PATH = L"Assets/sound/BossBattleBGM.wav";	//�{�X�o�g����BGM�p�X
}

namespace Game
{
	GameSceneBGM::~GameSceneBGM()
	{
		//�Đ�����BGM�Ǝ��ɍĐ��\���BGM���폜
		DeleteGO(m_playingSoundSource);
		DeleteGO(m_nextSoundSource);
	}

	bool GameSceneBGM::Start()
	{
		//�^�C�g��BGM���쐬
		m_playingSoundSource = NewGO<SoundSource>(Priority::High);
		m_playingSoundSource->Init(START_BGM_PATH,SoundType::enBGM);
		m_playingSoundSource->SetVolume(m_playingSoundVolume);
		m_playingSoundSource->Play(true);
		return true;
	}

	void GameSceneBGM::Update()
	{
		//BGM�ύX�̏���
		SoundChangeUpdate();
	}

	void GameSceneBGM::SoundChange()
	{
		//���łɍĐ�����Ă���T�E���h���t�F�[�h�A�E�g�����Ȃ���A���ɍĐ�����T�E���h���t�F�[�h�C��������B
		m_isSoundChanging = true;

		m_nextSoundSource = NewGO<SoundSource>(Priority::High);

		//�Q�[���̃t�F�[�Y�ɂ���ĕ���
		switch (m_gameBGMPhase)
		{
		case enTitle://�^�C�g��BGM�Ȃ�
			//���ɍĐ�����̂̓o�g��BGM
			m_nextSoundSource->Init(BATTLE_BGM_PATH, SoundType::enBGM);
			m_gameBGMPhase = enBattle;
			break;
		case enBattle://�ʏ�o�g��BGM�Ȃ�
			//���ɍĐ�����̂̓{�X�o�g��BGM
			m_nextSoundSource->Init(BOSSBATTLE_BGM_PATH, SoundType::enBGM);
			break;
		default:
			break;
		}

		//���ɍĐ�����BGM�̃{�����[�����Z�b�g
		m_nextSoundSource->SetVolume(m_nextSoundVolume);
		m_nextSoundSource->Play(true);
	}

	void GameSceneBGM::SoundChangeUpdate()
	{
		//BGM�ύX���łȂ��Ȃ牽�����Ȃ�
		if (m_isSoundChanging == false)
		{
			return;
		}

		//���ݍĐ�����BGM�̃{�����[���������Ă���
		m_playingSoundVolume -= m_soundChangeRate;
		//���ɍĐ�����BGM�̃{�����[�����グ�Ă���
		m_nextSoundVolume += m_soundChangeRate;

		//���ɍĐ�����BGM�̃{�����[�����ő�ɂȂ�����
		if (m_nextSoundVolume >= SOUND_VOLUME_MAX)
		{
			//�v���C��������BGM�̉��ʂ�0�Ȃ̂ō폜����
			DeleteGO(m_playingSoundSource);

			//�|�C���^���Đ�����BGM�ɕt���ւ���
			m_playingSoundSource = m_nextSoundSource;

			m_nextSoundSource = nullptr;

			//�t���ւ����̂ōĐ�����BGM�̃{�����[�����ő��
			m_playingSoundVolume = SOUND_VOLUME_MAX;

			//���ɍĐ�����BGM�̃{�����[���͍ŏ���
			m_nextSoundVolume = SOUND_VOLUME_MIN;

			//�Đ�����BGM�̃{�����[�����Z�b�g
			m_playingSoundSource->SetVolume(m_playingSoundVolume);

			//�ύX�������I��
			m_isSoundChanging = false;
			return;
		}

		//�ύX�������Ȃ�Đ���BGM�Ǝ��ɍĐ�����BGM�̉��ʂ��Z�b�g
		m_playingSoundSource->SetVolume(m_playingSoundVolume);
		m_nextSoundSource->SetVolume(m_nextSoundVolume);
	}
}
