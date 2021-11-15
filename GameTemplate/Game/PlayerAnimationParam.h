#pragma once
#include "stdafx.h"

namespace Game
{

	//�v���C���[�̃A�j���[�V�����ꗗ
	enum PlayerAnimationEnum
	{
		enAnim_Idle,//�ҋ@
		enAnim_Walk,//���s
		enAnim_Run,//����
		enAnim_Attack,//�R���{1
		enAnim_Attack2,//�R���{2
		enAnim_Attack3,//�R���{3
		enAnim_Guard,//�K�[�h
		enAnim_CutMode,//�ؒf���[�h
		enAnim_Num//�A�j���[�V�����̐�
	};

	//�v���C���[�̃A�j���[�V�����J�ڂɎg���p�����[�^�[
	struct PlayerAnimationParam
	{
		bool isWalking = false;
		bool isRunning = false;
		bool isGuarding = false;
		bool isCutMode = false;
		int comboNum = 0;
		int attackingTime = 0;
		Vector3 playerPosition = Vector3::Zero;
	};
}