#include "stdafx.h"
#include "DashTutorial.h"

namespace
{
	const char* SPRITE_PATH = "Assets/Image/DashTutorial.dds";				//�_�b�V���`���[�g���A���̃X�v���C�g�p�X
	const UINT SPRITE_WIDTH = 180;											//�_�b�V���`���[�g���A���̕�
	const UINT SPRITE_HEIGHT = 40;											//�_�b�V���`���[�g���A���̍���
	const Vector2 SPRITE_PIVOT = { 0.0f,0.0f };								//�_�b�V���`���[�g���A���̃s�{�b�g
	const Vector3 SPRITE_STARTPOSITION = { -730.0f,-300.0f,0.0f };			//�_�b�V���`���[�g���A���̏������W
	const Vector3 SPRITE_POSITION = { -550.0f,-300.0f,0.0f };				//�_�b�V���`���[�g���A���̍��W
	const int FRAME_MAX = 15;												//�_�b�V���`���[�g���A���̍ő�ړ��t���[��
	const wchar_t* APPEAR_SOUND_PATH = L"Assets/sound/TutorialAppearSE.wav";//�_�b�V���`���[�g���A���̏o�����ʉ��p�X
}

namespace Game
{
	DashTutorial::~DashTutorial()
	{
		//�_�b�V���`���[�g���A���̃X�v���C�g���폜
		DeleteGO(m_spriteRender);
	}

	bool DashTutorial::Start()
	{
		//�_�b�V���`���[�g���A���̃X�v���C�g�𐶐�
		m_spriteRender = NewGO<SpriteRender>(Priority::High);
		m_spriteRender->Init(SPRITE_PATH, SPRITE_WIDTH, SPRITE_HEIGHT);
		m_spriteRender->SetPivot(SPRITE_PIVOT);
		m_spriteRender->SetPosition(SPRITE_POSITION);

		//�����������̉����Đ�
		SoundOneShotPlay(APPEAR_SOUND_PATH);

		return true;
	}

	void DashTutorial::Update()
	{
		//������������X�v���C�g���|�b�v�A�b�v������
		m_popUpProgress++;

		m_popUpProgress = min(m_popUpProgress, FRAME_MAX);

		float rate = static_cast<float>(m_popUpProgress) / FRAME_MAX;
		Vector3 position;
		position.Lerp(rate, SPRITE_STARTPOSITION, SPRITE_POSITION);
		m_spriteRender->SetPosition(position);
	}
}
