#include "stdafx.h"
#include "PlayerAttack.h"
#include "PlayerAttackSound.h"
#include "EnemyMissile.h"
#include "BossTank.h"

namespace
{
	const char* PATH_HITBOXMODEL = "Assets/modelData/CutIndicator.tkm";
	const int TIME_ATTACK_END = 60;
	const int TIME_COMBO_ACCEPTABLE = 20;
	const int TIME_ATTACK_COLLISION = 14;
	const int MAX_CUT_NUM = 2;
	const float ATTACK_RANGE = 600.0f;
	const float ATTACK_DAMAGE = 25.0f;
	const Vector3 PLAYER_CUTPOINT = { 0.0f,120.0f,0.0f };
	const float PLAYER_CUTDEG = 0.7f;
	const Vector3 ATTACK_CUT_NORMAL[3] = {
		{0.1f,0.9f,-0.1f},
		{0.5f,0.5f,0.2f},
		{0.3f,0.9f,0.1f}
	};

	const Vector3 ATTACK_CUT_FORCE[3] = {
		{0.9f,0.1f,-0.1f},
		{-0.5f,0.5f,0.2f},
		{0.9f,0.3f,0.1f}
	};
	const EnButton BUTTON_ATTACK = enButtonX;
}

namespace Game
{
	PlayerAttack::~PlayerAttack()
	{
	}

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

				Vector3 cutPoint = PLAYER_CUTPOINT;
				cutPoint += playerPosition;

				Vector3 cutNormal = ATTACK_CUT_NORMAL[m_comboNum - 1];
				cutNormal.Normalize();

				toMoveRot.Apply(cutNormal);

				Vector3 cutForce = ATTACK_CUT_FORCE[m_comboNum - 1];
				cutForce.Normalize();

				toMoveRot.Apply(cutForce);
				
				Vector3 front = Vector3::Front;
				toMoveRot.Apply(front);

				bool hitCheck = false;

				ModelCutManager::GetInstance()->QueryCut(cutNormal, cutForce * 30,[cutPoint, front, &hitCheck](const SkinModelRender* cutObject)->bool
					{
						Vector3 distance = cutObject->GetPosition() - cutPoint;

						Vector3 toCutObject = distance;
						toCutObject.Normalize();

						bool isInRange = false;
						float dot = Dot(toCutObject, front);
						if (dot >= PLAYER_CUTDEG)
						{
							isInRange = true;
						}

						if (distance.LengthSq() < ATTACK_RANGE * ATTACK_RANGE && cutObject->GetDivideNum() <= MAX_CUT_NUM && isInRange)
						{
							hitCheck |= true;

							return true;
						}
						return false;
					}
				);

				//�{�X�ɋ߂���΃_���[�W��^����
				//TODO:�{�X�̎�ނ������邱�Ƃ��\�z�����̂Ŋ��N���X��p�ӂ���?
				//TODO:�G�ɂ���đ傫�����Ⴄ�̂�ATTACK_RANGE�Ƀ{�X���g�̑傫���ɍ��킹�ċ�����ǉ�����
				//TODO:�~�T�C�����̏d�v�łȂ��G�I�u�W�F�N�g�͒ʏ�U���ł��a���悤�ɂ���?
				BossTank* bossTank = FindGO<BossTank>("bosstank");

				if (bossTank != nullptr)
				{
					Vector3 distance = playerPosition - bossTank->GetPosition();

					if (distance.LengthSq() < ATTACK_RANGE * ATTACK_RANGE)
					{
						hitCheck |= true;
						bossTank->Damage(ATTACK_DAMAGE);
					}
				}

				if (hitCheck == false)
				{
					PlayerAttackSound playerAttackSound;
					playerAttackSound.PlayMissSound(m_comboNum - 1);
				}
				else
				{
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