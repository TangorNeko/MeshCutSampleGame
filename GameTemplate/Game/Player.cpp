#include "stdafx.h"
#include "Player.h"
#include "GameOverNotifier.h"
#include "BossTank.h"

namespace
{
	const char* PATH_PLAYER_MODEL = "Assets/modelData/Player.tkm";		//�v���C���[�̃��f���p�X
	const char* PATH_PLAYER_SKELETON = "Assets/modelData/Player.tks";	//�v���C���[�̃��f���̃X�P���g���p�X
	const int FINISH_CAMERA_START = 50;									//�g�h���J�����̉��o�J�n�t���[��
	const int PLAYER_HP_MAX = 1000;										//�v���C���[�̍ő�HP
}

namespace Game
{
	Player::~Player()
	{
		//���f�����폜
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

		//�ؒf���[�h���łȂ����͈ړ��\
		if (m_eventCut == false)
		{
			m_eventCut = m_playerMove.Move(m_position,m_playerAnimationParam);
		}
		else
		{
			m_playerAnimationParam.playerState = PlayerAnimationParam::enCutMode;
		}

		//�ؒf�C�x���g�̃A�b�v�f�[�g
		m_playerCut.SetCutEvent(m_eventCut,m_position);

		//�g�h���J�������Ȃ�g�h���J�E���g�𑝉�
		if (m_isFinishCamera == true)
		{
			m_finishCount++;
		}

		//�g�h���J�����̉��o�J�n�t���[�����Ȃ�
		if (m_finishCount >= FINISH_CAMERA_START)
		{
			//�g�h���J����
			m_playerCamera.UpdateFinishCamera(m_playerAnimationParam);
		}
		//�ؒf���[�h�łȂ��Ȃ�
		else if (m_eventCut == false)
		{
			//�ʏ�̃J�����̈ړ�
			m_playerCamera.Update(m_position);
		}
		//�ؒf���[�h�Ȃ�
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

		//�ؒf���[�h���łȂ��Ȃ�
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
		//HP����_���[�W�ʕ����炷
		m_hp -= damage;

		//�̗͂�0�ȉ��ɂȂ�����
		if (m_hp <= 0)
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
		//�̗͂���
		m_hp += healValue;

		//�̗͂̍ő�l�ȏ�Ȃ�
		if (m_hp > PLAYER_HP_MAX)
		{
			//�̗͂��ő�l��
			m_hp = PLAYER_HP_MAX;
		}
	}

	void Player::NoticeMissileMoveStart()
	{
		//�~�T�C���ړ������̊J�n��ʒm
		m_playerMove.NoticeMissileMoveStart();

		//�{�X�̖C�g�̐ؒf������
		BossTank* bossTank = FindGO<BossTank>("bosstank");
		bossTank->AllowCannonCut();
	}

	void Player::NoticeFrontMoveStart()
	{
		//�{�X�̐��ʂւ̈ړ������̊J�n��ʒm
		m_playerMove.NoticeFrontMoveStart();

		//�{�X�̎ԑ̂ƖC���̐ؒf������
		BossTank* bossTank = FindGO<BossTank>("bosstank");
		bossTank->AllowBodyCut();
	}
}
