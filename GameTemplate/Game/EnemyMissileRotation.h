#pragma once

namespace Game
{
	class EnemyMissileRotation
	{
	public:
		/**
		 * @brief ��]�����̃A�b�v�f�[�g
		 * @param qRot �~�T�C���̉�]
		 * @param direction �~�T�C���̌���
		*/
		void Update(Quaternion& qRot, const Vector3& direction);
	};
}

