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
		m_playerMove.Init(m_position);
		m_playerModel->SetPosition(m_position);

		//�v���C���[��HP�\��
		m_hpRender = NewGO<FontRender>(1);
		wchar_t buffer[256];
		swprintf_s(buffer, L"HP:%d",m_hp);
		m_hpRender->SetText(buffer);
		m_hpRender->SetShadowFlag(true);
		m_hpRender->SetShadowColor(SHADOWCOLOR_BLACK);
		return true;
	}

	void Player::Update()
	{
		//�v���C���[��HP�\��
		//TODO:�N���X����
		wchar_t buffer[256];
		swprintf_s(buffer, L"HP:%d", m_hp);
		m_hpRender->SetText(buffer);
		m_hpRender->SetPosition({ -600.0f,330.0f });

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

		//NOTE:���ōU�����f���Ɛؒf���f���������ɒǏ]�����Ă���@�ォ��g��Ȃ��悤�ɂ���
		if (m_playerAttack.GetModel() != nullptr)
		{
			m_playerMove.TurnModelToPlayerDirection(m_playerAttack.GetModel());
		}

		if (m_playerCut.GetModel() != nullptr)
		{
			m_playerMove.TurnModelToPlayerDirection(m_playerCut.GetModel());
		}


		//�U���̃A�b�v�f�[�g
		m_playerAttack.Update(m_position, m_playerAnimationParam);
		
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
