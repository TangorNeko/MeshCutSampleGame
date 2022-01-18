#pragma once

namespace Game
{
	struct BossTankStatus
	{
		Vector3 position = Vector3::Zero;				//�{�X�̍��W
		Quaternion baseRot = Quaternion::Identity;		//�ԑ̂̃N�H�[�^�j�I��
		Quaternion turretRot = Quaternion::Identity;	//�C���̃N�H�[�^�j�I��
		Quaternion trackingRot = Quaternion::Identity;	//�v���C���[�ǔ��p�̃N�H�[�^�j�I��
		float baseDeg = 0.0f;							//�ԑ̂̊p�x
		float turretDeg = 0.0f;							//�C���̊p�x
		int hp = 400;									//�̗�
		bool isCannonBreak = false;						//�C�g���󂳂ꂽ?
		bool isTurretBreak = false;						//�C�����󂳂ꂽ?
		bool isBaseBreak = false;						//�ԑ̂��󂳂ꂽ?
		bool isSummonMinions = false;					//�G���G����������?
		bool isStepAttack = false;						//�̗͔������̑���~�T�C���U�����s������?
	};
}

