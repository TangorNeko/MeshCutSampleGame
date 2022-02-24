#include "stdafx.h"
#include "CommandInput.h"
#include "Player.h"

namespace
{
	const char* COMMAND_SPRITE_PATH = "Assets/Image/BButtonSprite.dds";		//�R�}���h���͂̃X�v���C�g�̃p�X
	const UINT COMMAND_SPRITE_SIDE = 100;									//�R�}���h���̓X�v���C�g�̈�ӂ̒���
	const Vector4 COLOR_TRANSPARENT = {1.0f,1.0f,1.0f,0.0f};				//���ߐF
	const Vector4 COLOR_OPAQUE = { 1.0f,1.0f,1.0f,1.0f };					//�s�����F
	const int FADE_FRAME_MAX = 15;											//�t�F�[�h�̍ő�l
	const float FADE_RATE_PROGRESS = 1.0f;									//�t�F�[�h�̐i�s�x
	const Vector3 COMMAND_POSITION = {0.0f,-200.0f,0.0f};					//�R�}���h�X�v���C�g�̕\�����W
	const wchar_t* APPEAR_SOUND_PATH = L"Assets/sound/ButtonAppearSE.wav";	//�R�}���h���͂̏o�����̃t�@�C���p�X
	const wchar_t* SUCCESS_SOUND_PATH = L"Assets/sound/ButtonSuccessSE.wav";//�R�}���h���͐������̃t�@�C���p�X
	const float COMMAND_SCALE_Z = 1.0f;										//Z�����̊g�嗦
}

namespace Game
{
	CommandInput::~CommandInput()
	{
		//�X�v���C�g���폜
		DeleteGO(m_commandSprite);
	}

	bool CommandInput::Start()
	{
		//�R�}���h���͂̃X�v���C�g���쐬
		m_commandSprite = NewGO<SpriteRender>(Priority::High);
		m_commandSprite->Init(COMMAND_SPRITE_PATH, COMMAND_SPRITE_SIDE, COMMAND_SPRITE_SIDE);
		m_commandSprite->SetPosition(COMMAND_POSITION);

		//�o�����̃T�E���h���Đ�
		SoundOneShotPlay(APPEAR_SOUND_PATH);
		return true;
	}

	void CommandInput::Update()
	{
		if (g_pad[0]->IsTrigger(enButtonB) && m_isSuccess == false)
		{
			//�R�}���h���͂ɐ��������琬������炷
			m_isSuccess = true;
			SoundOneShotPlay(SUCCESS_SOUND_PATH);
		}

		if (m_isSuccess)
		{
			//�R�}���h���͂ɐ������Ă����珙�X�Ƀt�F�[�h�A�E�g
			FadeOut();
		}
	}

	void CommandInput::Pause()
	{
		if (g_pad[0]->IsTrigger(enButtonB) && m_isSuccess == false)
		{
			//�R�}���h���͂ɐ��������琬������炷
			m_isSuccess = true;
			SoundOneShotPlay(SUCCESS_SOUND_PATH);

			//�|�[�Y������
			GameObjectManager::GetInstance()->SetPauseFlag(false);

			//�v���C���[�Ƀ~�T�C���ړ��̐�����ʒm
			Player* player = FindGO<Player>("player");
			player->MissileMoveSuccess();
		}
	}

	void CommandInput::FadeOut()
	{
		//�o�߃t���[�����𑝉�
		m_fadeFrame++;

		//�ő�l��ݒ�
		m_fadeFrame = min(m_fadeFrame, FADE_FRAME_MAX);

		//���݂̃t�F�[�h�����v�Z
		float fadeRate = static_cast<float>(m_fadeFrame) / FADE_FRAME_MAX;

		//�t�F�[�h���ɍ��킹�ăX�v���C�g���g��
		float spriteScale = fadeRate + FADE_RATE_PROGRESS;
		m_commandSprite->SetScale({ spriteScale ,spriteScale,COMMAND_SCALE_Z });

		//�t�F�[�h���ɍ��킹�ē��߂�����
		Vector4 mulColor;
		mulColor.Lerp(fadeRate, COLOR_OPAQUE, COLOR_TRANSPARENT);
		m_commandSprite->SetMulColor(mulColor);
	}
}
