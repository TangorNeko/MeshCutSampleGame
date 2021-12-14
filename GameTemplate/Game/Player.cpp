#include "stdafx.h"
#include "Player.h"

namespace
{
	const char* PATH_PLAYER_MODEL = "Assets/modelData/Samurai.tkm";
	const char* PATH_PLAYER_SKELETON = "Assets/modelData/Samurai.tks";
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

		//�����̌������Z�b�g
		m_playerMove.SetPlayerDirection(Vector3::Back);
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
		
		//�v���C���[�̌��������]���v�Z
		m_playerMove.CalcToModelDirectionQRot();

		//�v���C���[�̌����Ƀ��f����������
		m_playerModel->SetRotation(m_playerMove.GetPlayerDirectionRot());

		//�U���̃A�b�v�f�[�g
		m_playerAttack.Update(m_position, m_playerAnimationParam,m_playerMove.GetPlayerDirectionRot());
		
		//�ؒf�̃A�b�v�f�[�g
		m_playerCut.Update(m_position, m_playerMove.GetPlayerDirectionRot());


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

	void Player::Heal(int healValue)
	{
		m_hp += healValue;

		if (m_hp > 1000)
		{
			m_hp = 1000;
		}
	}
}
