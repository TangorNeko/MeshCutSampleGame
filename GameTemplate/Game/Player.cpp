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
		//NOTE:���BLB1�����Ă��鎞�͈ړ������Ȃ�
		if (g_pad[0]->IsPress(enButtonLB1) == false)
		{
			m_playerMove.Move(m_position);
		}

		//�ړ������Ƀ��f����������
		m_playerMove.TurnModelToMoveDirection(m_playerModel);

		//NOTE:���ōU�����f���Ɛؒf���f�����ړ��ɒǏ]�����Ă���@�ォ��g��Ȃ��悤�ɂ���
		m_playerMove.TurnModelToMoveDirection(m_playerAttack.GetModel());
		m_playerMove.TurnModelToMoveDirection(m_playerCut.GetModel());


		//�U���̃A�b�v�f�[�g
		m_playerAttack.Update(m_position);
		
		//�ؒf�̃A�b�v�f�[�g
		m_playerCut.Update(m_position, m_playerMove.CalcToModelDirectionQRot());

		//NOTE:���BLB1�����Ă��鎞�̓J�����𓮂����Ȃ�
		if (g_pad[0]->IsPress(enButtonLB1) == false)
		{
			//�J�����̈ړ�
			m_playerCamera.Update(m_position);
		}
		//TODO:LB1�����������̓v���C���[�̌����ɃJ�������ړ�

		//�A�j���[�V�����̃A�b�v�f�[�g
		m_playerAnimation.Update(m_playerModel);

		//�ړ��̌��ʂ����f���ɔ��f
		m_playerModel->SetPosition(m_position);
	}
}
