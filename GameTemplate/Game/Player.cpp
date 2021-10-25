#include "stdafx.h"
#include "Player.h"

namespace
{
	const char* PATH_PLAYER_MODEL = "Assets/modelData/unityChan.tkm";
	const char* PATH_PLAYER_SKELETON = "Assets/modelData/unityChan.tks";
}

namespace Game
{
	Player::~Player()
	{

	}

	bool Player::Start()
	{
		//���f���𐶐�
		m_playerModel = NewGO<SkinModelRender>(0);

		//�A�j���[�V������������
		m_playerAnimation.InitAnim();

		//���f���̏�����
		m_playerModel->Init(PATH_PLAYER_MODEL, PATH_PLAYER_SKELETON,m_playerAnimation.GetAnimationClip(),m_playerAnimation.GetAnimClipNum());
		m_playerMove.Init(m_position);
		m_playerModel->SetPosition(m_position);

		return true;
	}

	void Player::Update()
	{
		//�ړ�
		m_playerMove.Move(m_position);

		//�ړ������Ƀ��f����������
		m_playerMove.TurnModelToMoveDirection(m_playerModel);

		//�J�����̈ړ�
		m_playerCamera.Update(m_position);

		//�A�j���[�V�����̃A�b�v�f�[�g
		m_playerAnimation.Update(m_playerModel);

		//�ړ��̌��ʂ����f���ɔ��f
		m_playerModel->SetPosition(m_position);
	}
}
