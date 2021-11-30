#include "stdafx.h"
#include "PlayerAttack.h"
#include "EnemyMissile.h"
#include "BossTank.h"

namespace
{
	const char* PATH_HITBOXMODEL = "Assets/modelData/ball.tkm";
	const int TIME_ATTACK_END = 60;
	const int TIME_COMBO_ACCEPTABLE = 20;
	const int TIME_ATTACK_COLLISION = 14;
	const float ATTACK_RANGE = 500.0f;
	const float ATTACK_DAMAGE = 25.0f;
	const EnButton BUTTON_ATTACK = enButtonX;
}

namespace Game
{
	PlayerAttack::~PlayerAttack()
	{
		/*
		if (m_testHitBox != nullptr)
		{
			DeleteGO(m_testHitBox);
		}
		*/
	}

	void PlayerAttack::Update(const Vector3& playerPosition, PlayerAnimationParam& animParam)
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

			//�����蔻�肪�����
			/*
			if (m_testHitBox != nullptr)
			{
				//�����蔻��̍��W���v���C���[�̈ʒu�ɃZ�b�g
				m_testHitBox->SetPosition(playerPosition);
			}
			*/

			//�U���̓����蔻����쐬���鎞�ԂɂȂ�����
			if (m_attackTime == TIME_ATTACK_COLLISION)
			{
				//TODO:�g���K�[�̓����蔻����쐬
				/*
				m_testHitBox = NewGO<SkinModelRender>(0);
				m_testHitBox->Init(PATH_HITBOXMODEL);
				m_testHitBox->SetPosition(playerPosition);
				*/

				//�{�X�ɋ߂���΃_���[�W��^����
				//TODO:�{�X�̎�ނ������邱�Ƃ��\�z�����̂Ŋ��N���X��p�ӂ���?
				//TODO:�G�ɂ���đ傫�����Ⴄ�̂�ATTACK_RANGE�Ƀ{�X���g�̑傫���ɍ��킹�ċ�����ǉ�����
				//TODO:�~�T�C�����̏d�v�łȂ��G�I�u�W�F�N�g�͒ʏ�U���ł��a���悤�ɂ���?
				BossTank* bossTank = FindGO<BossTank>("bosstank");
				Vector3 distance = playerPosition - bossTank->GetPosition();

				if (distance.LengthSq() < ATTACK_RANGE * ATTACK_RANGE)
				{
					bossTank->Damage(ATTACK_DAMAGE);
				}
			}

			//1,2�i�ڂ܂ł̍U���J�n�R���{��t���Ԉȓ��ŁA�U���{�^����������
			if (g_pad[0]->IsTrigger(BUTTON_ATTACK) && m_attackTime > TIME_COMBO_ACCEPTABLE && m_comboNum <= 2)
			{
				//���̃R���{�Ɉڂ邽�ߓ����蔻��͍폜
				/*
				DeleteGO(m_testHitBox);
				m_testHitBox = nullptr;
				*/

				//�R���{�i�����C���N�������g
				m_comboNum++;

				//�U�����Ԃ����Z�b�g
				m_attackTime = 0;
			}

			//�U���I���t���[���𒴂���Ɗ��S�ɍU���I��
			if (m_attackTime > TIME_ATTACK_END)
			{
				//�����蔻����폜
				/*
				DeleteGO(m_testHitBox);
				m_testHitBox = nullptr;
				*/

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