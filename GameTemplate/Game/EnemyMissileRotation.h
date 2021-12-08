#pragma once

namespace Game
{
	class EnemyMissileRotation
	{
	public:
		/**
		 * @brief 回転処理のアップデート
		 * @param qRot ミサイルの回転
		 * @param direction ミサイルの向き
		*/
		void Update(Quaternion& qRot, const Vector3& direction);
	};
}

