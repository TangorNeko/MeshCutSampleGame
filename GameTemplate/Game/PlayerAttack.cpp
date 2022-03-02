#include "stdafx.h"
#include "PlayerAttack.h"
#include "PlayerAttackSound.h"
#include "EnemyMissile.h"
#include "BossTank.h"

namespace
{
	const int TIME_ATTACK_END = 60;							//�U���̏I���t���[��
	const int TIME_COMBO_ACCEPTABLE = 20;					//���R���{�̎�t�t���[��
	const int TIME_ATTACK_COLLISION = 14;					//�U���̓����蔻��̔����t���[��
	const int MAX_CUT_NUM = 2;								//�ő�ؒf��
	const float ATTACK_RANGE = 600.0f;						//�U���̎˒�
	const int ATTACK_DAMAGE = 25.0f;						//�U���̃_���[�W
	const Vector3 PLAYER_CUTPOINT = { 0.0f,120.0f,0.0f };	//�ʏ�U���̐ؒf�̋N�_
	const float PLAYER_CUTDEG = 0.7f;						//�v���C���[�̐��ʂ̊p�x
	const Vector3 ATTACK_CUT_NORMAL[3] = {					//�e�R���{�̐ؒf�ʂ̖@��
		{0.1f,0.9f,-0.1f},
		{0.5f,0.5f,0.2f},
		{0.3f,0.9f,0.1f}
	};

	const Vector3 ATTACK_CUT_FORCE[3] = {					//�e�R���{�̐ؒf���̗�
		{0.9f,0.1f,-0.1f},
		{-0.5f,0.5f,0.2f},
		{0.9f,0.3f,0.1f}
	};
	const EnButton BUTTON_ATTACK = enButtonX;				//�U���{�^��
}

namespace Game
{
	void PlayerAttack::Update(const Vector3& playerPosition, PlayerAnimationParam& animParam, const Quaternion& toMoveRot)
	{
		//�U�����Ă��Ȃ���ԂōU���{�^����������
		if (g_pad[0]->IsTrigger(BUTTON_ATTACK) && m_comboNum == 0)
		{
			//�R���{�i�����C���N�������g
			m_comboNum++;
		}

		//�R���{1�ȍ~���o���Ă���Ȃ�
		if (m_comboNum >= 1)
		{
			//�U���̃J�E���g���C���N�������g
			m_attackTime++;

			//�U���̓����蔻����쐬���鎞�ԂɂȂ�����
			if (m_attackTime == TIME_ATTACK_COLLISION)
			{
				//�ؒf�̋N�_���v�Z
				Vector3 cutPoint = PLAYER_CUTPOINT;
				cutPoint += playerPosition;

				//�R���{�i������ؒf�ʂ̖@�����擾
				Vector3 cutNormal = ATTACK_CUT_NORMAL[m_comboNum - 1];
				cutNormal.Normalize();

				//�@���Ƀv���C���[�̉�]��K�p
				toMoveRot.Apply(cutNormal);

				//�R���{�i������ؒf���̗͂��擾
				Vector3 cutForce = ATTACK_CUT_FORCE[m_comboNum - 1];
				cutForce.Normalize();

				//�ؒf���̗͂Ƀv���C���[�̉�]��K�p
				toMoveRot.Apply(cutForce);
				
				//�v���C���[�̐��ʂ��擾
				Vector3 front = Vector3::Front;
				toMoveRot.Apply(front);

				//�U�����q�b�g������
				bool hitCheck = false;

				//�ؒf�N�G��
				ModelCutManager::GetInstance()->QueryCut(cutNormal, cutForce * 30,[cutPoint, front, &hitCheck](const SkinModelRender* cutObject)->bool
					{
						//�v���C���[�̐ؒf�I�u�W�F�N�g�Ƃ̋������v�Z
						Vector3 distance = cutObject->GetPosition() - cutPoint;

						Vector3 toCutObject = distance;
						toCutObject.Normalize();

						//�v���C���[�̐��ʂɐؒf�I�u�W�F�N�g�����邩
						bool isInRange = false;
						float dot = Dot(toCutObject, front);
						if (dot >= PLAYER_CUTDEG)
						{
							isInRange = true;
						}

						//�ؒf�͈͓��ŁA�v���C���[�̐��ʂɂ���A�ؒf�񐔂����Ȃ��Ȃ�
						if (distance.LengthSq() < ATTACK_RANGE * ATTACK_RANGE && cutObject->GetDivideNum() <= MAX_CUT_NUM && isInRange)
						{
							//�U�����q�b�g����
							hitCheck |= true;

							//�ؒf
							return true;
						}

						//�ؒf���Ȃ�
						return false;
					}
				);

				//�{�X�̃C���X�^���X���擾
				BossTank* bossTank = FindGO<BossTank>("bosstank");

				//�{�X�����݂����ꍇ
				if (bossTank != nullptr)
				{
					Vector3 distance = playerPosition - bossTank->GetPosition();

					//�{�X�ɋ߂���΃_���[�W��^����
					if (distance.LengthSq() < ATTACK_RANGE * ATTACK_RANGE)
					{
						//�U�����q�b�g����
						hitCheck |= true;
						bossTank->Damage(ATTACK_DAMAGE);
					}
				}

				//�U�����ǂ��ɂ��q�b�g���Ă��Ȃ��ꍇ
				if (hitCheck == false)
				{
					//��U����ʉ����Đ�
					PlayerAttackSound playerAttackSound;
					playerAttackSound.PlayMissSound(m_comboNum - 1);
				}
				//�{�X�܂��͐ؒf�I�u�W�F�N�g�Ƀq�b�g����
				else
				{
					//�q�b�g���ʉ����Đ�
					PlayerAttackSound playerAttackSound;
					playerAttackSound.PlayHitSound(m_comboNum - 1);
				}
			}

			//1,2�i�ڂ܂ł̍U���J�n�R���{��t���Ԉȓ��ŁA�U���{�^����������
			if (g_pad[0]->IsTrigger(BUTTON_ATTACK) && m_attackTime > TIME_COMBO_ACCEPTABLE && m_comboNum <= 2)
			{
				//�R���{�i�����C���N�������g
				m_comboNum++;

				//�U�����Ԃ����Z�b�g
				m_attackTime = 0;
			}

			//�U���I���t���[���𒴂���Ɗ��S�ɍU���I��
			if (m_attackTime > TIME_ATTACK_END)
			{
				//�R���{�i���ƍU�����Ԃ����Z�b�g
				m_comboNum = 0;
				m_attackTime = 0;
			}
		}

		//�A�j���[�V�����p
		animParam.comboNum = m_comboNum;
		animParam.attackingTime = m_attackTime;
	}
}