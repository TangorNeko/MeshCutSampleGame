#include "stdafx.h"
#include "EnemyMissileMove.h"
#include "Player.h"

namespace
{
	const float MISSILE_SPEED = 30.0f;
	const Vector3 ERROR_VECTOR3 = { 1000.0f,1000.0f,1000.0f };
	const Vector3 PLAYER_TO_CUTPOINT = { 0.0f,120.0f,0.0f };
}

namespace Game
{
	EnemyMissileMove::EnemyMissileMove()
	{
	}

	void EnemyMissileMove::Update(Vector3& position)
	{
		//�ړ����Ԃ��C���N�������g
		m_moveTime++;

		//�v���C���[���a��₷���悤�ɒǔ�����n�_�̍����͐ؒf�̒��S�̍����ɂ���
		Vector3 targetPos = m_trackingPlayer->GetPosition() + PLAYER_TO_CUTPOINT;

		//�~�T�C���̍��W����^�[�Q�b�g�̍��W�ւ̃x�N�g��(���K�����Ă��Ȃ��ړ�����)�����߂�
		m_moveDirection = targetPos - position;

		//�Q�̍��W���߂�����ꍇ�͌����ɃG���[�l������return
		//NOTE:����ɏ������i�񂾏ꍇm_moveDirection�͐��K�����꒷��1�ɂȂ邽��
		//�G���[�l�Ƃ��Ē������傫�ȃx�N�g�����i�[���Ă���
		if (m_moveDirection.LengthSq() < FLT_MIN)
		{
			m_moveDirection = ERROR_VECTOR3;
			return;
		}

		//�v���C���[�ւ̃x�N�g���𐳋K��
		m_moveDirection.Normalize();

		CalcAvg(m_moveDirection);

		//���݂̍��W����v���C���[�����ɃX�s�[�h���ړ�������
		position += m_moveDirection * MISSILE_SPEED;
	}

	void EnemyMissileMove::CalcAvg(Vector3& direction)
	{
		//���������X�g�̌��ɋl�߂Ă���
		m_pastDirectionList.push_back(direction);

		int listSize = static_cast<int>(m_pastDirectionList.size());
		//���X�g�̃T�C�Y��15��葽���Ȃ�
		if (listSize > 15)
		{
			m_pastDirectionList.pop_front();
			listSize--;
		}

		Vector3 directionSum = Vector3::Zero;
		for (auto& pastDirection : m_pastDirectionList)
		{
			directionSum += pastDirection;
		}
		
		direction = directionSum / listSize;
	}
}