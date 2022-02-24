#include "stdafx.h"
#include "Player.h"
#include "GameOverNotifier.h"
#include "BossTank.h"

namespace
{
	const char* PATH_PLAYER_MODEL = "Assets/modelData/Player.tkm";
	const char* PATH_PLAYER_SKELETON = "Assets/modelData/Player.tks";
	const int FINISH_CAMERA_START = 50;
	const int PLAYER_HP_MAX = 1000;
}

namespace Game
{
	Player::~Player()
	{
		DeleteGO(m_playerModel);
	}

	bool Player::Start()
	{
		//���f���𐶐�
		m_playerModel = NewGO<SkinModelRender>(Priority::High);

		//�A�j���[�V������������
		m_playerAnimation.InitAnim();

		//���f���̏�����
		m_playerModel->Init(PATH_PLAYER_MODEL, PATH_PLAYER_SKELETON,m_playerAnimation.GetAnimationClip(),m_playerAnimation.GetAnimClipNum(),enModelUpAxisZ);
		m_playerModel->SetPosition(m_position);

		//�v���C���[�̈ړ��N���X�̏�����
		m_playerMove.Init(m_position);

		//�����̌������Z�b�g
		m_playerMove.SetPlayerDirection(Vector3::Back);
		return true;
	}

	void Player::Update()
	{
		//�v���C���[��HP�\���̍X�V
		m_playerDisplay.Update(m_hp);

		//�ړ�
		//NOTE:���BLB1�����Ă��鎞�͈ړ������Ȃ�
		if (m_eventCut == false)
		{
			m_eventCut = m_playerMove.Move(m_position,m_playerAnimationParam);
		}
		else
		{
			m_playerAnimationParam.playerState = PlayerAnimationParam::enCutMode;
		}

		m_playerCut.SetCutEvent(m_eventCut,m_position);

		if (m_isFinishCamera == true)
		{
			m_finishCount++;
		}

		if (m_finishCount >= FINISH_CAMERA_START)
		{
			m_playerCamera.UpdateFinishCamera(m_playerAnimationParam);
		}
		else if (m_eventCut == false)
		{
			//�J�����̈ړ�
			m_playerCamera.Update(m_position);
		}
		else
		{
			//�ؒf���[�h�p�J�����̈ړ�
			Vector3 newDirection = m_playerCamera.UpdateCutMode(m_position, m_playerMove.GetPlayerDirection());

			//�ؒf���[�h�J�����Ŏ��_��ύX����ƃv���C���[�̌������ύX�����̂ŐV�����������i�[
			m_playerMove.SetPlayerDirection(newDirection);
		}
		
		//�v���C���[�̌��������]���v�Z
		m_playerMove.CalcToModelDirectionQRot();

		//�v���C���[�̌����Ƀ��f����������
		m_playerModel->SetRotation(m_playerMove.GetPlayerDirectionRot());

		if (m_eventCut == false)
		{
			//�U���̃A�b�v�f�[�g
			m_playerAttack.Update(m_position, m_playerAnimationParam, m_playerMove.GetPlayerDirectionRot());
		}

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
			//���S����
			m_hp = 0;

			//�v���C���[�̈ړ����֎~
			m_playerMove.DisableMove();
			//�v���C���[�̃A�j���[�V�����̏�Ԃ����S��Ԃɂ���B
			m_playerAnimationParam.playerState = PlayerAnimationParam::enDead;

			//�Q�[���I�[�o�[��ʒm
			GameOverNotifier gameOverNotifer;
			gameOverNotifer.NotifyGameOver();
		}
	}

	void Player::Heal(int healValue)
	{
		m_hp += healValue;

		if (m_hp > PLAYER_HP_MAX)
		{
			m_hp = PLAYER_HP_MAX;
		}
	}

	void Player::NoticeMissileMoveStart()
	{
		m_playerMove.NoticeMissileMoveStart();

		BossTank* bossTank = FindGO<BossTank>("bosstank");
		bossTank->AllowCannonCut();
	}

	void Player::NoticeFrontMoveStart()
	{
		m_playerMove.NoticeFrontMoveStart();

		BossTank* bossTank = FindGO<BossTank>("bosstank");
		bossTank->AllowBodyCut();
	}
}
