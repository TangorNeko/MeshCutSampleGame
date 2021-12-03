#include "stdafx.h"
#include "Player.h"

namespace
{
	const char* PATH_PLAYER_MODEL = "Assets/modelData/Samurai.tkm";
	const char* PATH_PLAYER_SKELETON = "Assets/modelData/Samurai.tks";
	const Vector4 SHADOWCOLOR_BLACK = { 0.0f,0.0f,0.0f,1.0f };
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
		m_playerModel->Init(PATH_PLAYER_MODEL, PATH_PLAYER_SKELETON,m_playerAnimation.GetAnimationClip(),m_playerAnimation.GetAnimClipNum(),enModelUpAxisZ);
		m_playerModel->SetPosition(m_position);

		//�v���C���[�̈ړ��N���X�̏�����
		m_playerMove.Init(m_position);

		//�v���C���[��HP�\��
		m_playerDisplay.Init(m_hp);
	
		return true;
	}

	void Player::Update()
	{
		//�v���C���[��HP�\���̍X�V
		m_playerDisplay.Update(m_hp);

		bool isCutMode = g_pad[0]->IsPress(enButtonLB1);

		//�ړ�
		//NOTE:���BLB1�����Ă��鎞�͈ړ������Ȃ�
		if (isCutMode == false)
		{
			m_playerMove.Move(m_position,m_playerAnimationParam);
			m_playerAnimationParam.isCutMode = false;;
		}
		else
		{
			m_playerAnimationParam.isCutMode = true;
		}

		//TODO:���̃K�[�h�A�j���@�ォ��N���X����
		if (g_pad[0]->IsPress(enButtonRB1))
		{
			m_playerAnimationParam.isGuarding = true;
		}
		else
		{
			m_playerAnimationParam.isGuarding = false;
		}

		if (isCutMode == false)
		{
			//�J�����̈ړ�
			m_playerCamera.Update(m_position);
		}
		else
		{
			//�ؒf���[�h�p�J�����̈ړ�
			Vector3 newDirection = m_playerCamera.UpdateCutMode(m_position,m_playerMove.GetPlayerDirection());

			//�ؒf���[�h�J�����Ŏ��_��ύX����ƃv���C���[�̌������ύX�����̂ŐV�����������i�[
			m_playerMove.SetPlayerDirection(newDirection);
		}

		//�v���C���[�̌����Ƀ��f����������
		m_playerMove.TurnModelToPlayerDirection(m_playerModel);

		//NOTE:���Őؒf���f���������ɒǏ]�����Ă���@�ォ��g��Ȃ��悤�ɂ���
		if (m_playerCut.GetModel() != nullptr)
		{
			m_playerMove.TurnModelToPlayerDirection(m_playerCut.GetModel());
		}


		//�U���̃A�b�v�f�[�g
		m_playerAttack.Update(m_position, m_playerAnimationParam,m_playerMove.GetPlayerDirectionRot());
		
		//�ؒf�̃A�b�v�f�[�g
		m_playerCut.Update(m_position, m_playerMove.CalcToModelDirectionQRot());


		//�A�j���[�V�����̃A�b�v�f�[�g
		m_playerAnimation.Update(m_playerModel, m_playerAnimationParam);

		//�ړ��̌��ʂ����f���ɔ��f
		m_playerModel->SetPosition(m_position);
	}

	void Player::Damage(int damage)
	{
		m_hp -= damage;

		if (m_hp < 0)
		{
			//TODO:���S����
			m_hp = 0;
		}
	}
}
