#pragma once
#include "stdafx.h"

namespace Game
{

	//�v���C���[�̃A�j���[�V�����ꗗ
	enum PlayerAnimationEnum
	{
		enAnim_Idle,//�ҋ@
		enAnim_Walk,//���s
		enAnim_Attack,//�U��
		enAnim_Attack2,
		enAnim_Attack3,
		enAnim_Num//�A�j���[�V�����̐�
	};

	//�v���C���[�̃A�j���[�V�����J�ڂɎg���p�����[�^�[
	struct PlayerAnimationParam
	{
		bool isWalking = false;
		int attackNum = false;
		float attackingTime = 0.0f;
		Vector3 playerPosition = Vector3::Zero;
	};
}