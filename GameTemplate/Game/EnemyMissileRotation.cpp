#include "stdafx.h"
#include "EnemyMissileRotation.h"

namespace
{
	const float DIRECTION_ERROR_LENGTHSQ = 10.0f;
}

namespace Game
{
	void EnemyMissileRotation::Update(Quaternion& qRot, const Vector3& direction)
	{
		//向きがエラー値だった場合何もしない
		if (direction.LengthSq() > DIRECTION_ERROR_LENGTHSQ)
		{
			return;
		}

		//回転軸を求める
		Vector3 rotateAxis = Cross(Vector3::Up, direction);

		//外積の結果が不正だった場合何もしない
		if (rotateAxis.LengthSq() < 0.5)
		{
			return;
		}

		//念の為回転軸を正規化
		rotateAxis.Normalize();

		//回転角度を求める
		float rotDeg = Dot(Vector3::Up, direction);
		rotDeg = acos(rotDeg);
		rotDeg = Math::RadToDeg(rotDeg);

		//セット
		qRot.SetRotationDeg(rotateAxis, rotDeg);
	}
}