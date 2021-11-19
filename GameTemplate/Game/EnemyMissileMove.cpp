#include "stdafx.h"
#include "EnemyMissileMove.h"
#include "Player.h"

namespace
{
	const float MISSILE_SPEED = 5.0f;
	const Vector3 ERROR_VECTOR3 = { 1000.0f,1000.0f,1000.0f };
}

namespace Game
{
	void EnemyMissileMove::FindTarget()
	{
		m_trackingPlayer = FindGO<Player>("player");
	}

	void EnemyMissileMove::Update(Vector3& position)
	{
		//�ړ����Ԃ��C���N�������g
		m_moveTime++;

		//�~�T�C���̍��W����v���C���[�̍��W�ւ̃x�N�g��(���K�����Ă��Ȃ��ړ�����)�����߂�
		m_moveDirection = m_trackingPlayer->GetPosition() - position;

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

		//���݂̍��W����v���C���[�����ɃX�s�[�h���ړ�������
		position += m_moveDirection * MISSILE_SPEED;
	}
}