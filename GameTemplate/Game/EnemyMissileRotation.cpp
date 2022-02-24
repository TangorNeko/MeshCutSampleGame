#include "stdafx.h"
#include "EnemyMissileRotation.h"

namespace
{
	const float DIRECTION_ERROR_LENGTHSQ = 10.0f;	//�����̃G���[�l�̒����̓��̂������l
	const float ROTATEAXIS_ERROR_LENGTHSQ = 0.5f;	//��]���̃G���[�l�̒����̓��̂������l
}

namespace Game
{
	void EnemyMissileRotation::Update(Quaternion& qRot, const Vector3& direction)
	{
		//�������G���[�l�������ꍇ�������Ȃ�
		if (direction.LengthSq() > DIRECTION_ERROR_LENGTHSQ)
		{
			return;
		}

		//��]�������߂�
		Vector3 rotateAxis = Cross(Vector3::Up, direction);

		//�O�ς̌��ʂ��s���������ꍇ�������Ȃ�
		if (rotateAxis.LengthSq() < ROTATEAXIS_ERROR_LENGTHSQ)
		{
			return;
		}

		//�O�̈׉�]���𐳋K��
		rotateAxis.Normalize();

		//��]�p�x�����߂�
		float rotDeg = Dot(Vector3::Up, direction);
		rotDeg = acos(rotDeg);
		rotDeg = Math::RadToDeg(rotDeg);

		//��]���Z�b�g
		qRot.SetRotationDeg(rotateAxis, rotDeg);
	}
}