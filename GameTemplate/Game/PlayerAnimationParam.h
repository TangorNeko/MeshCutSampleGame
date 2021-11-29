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
		enAnim_KnockDown,//�m�b�N�_�E��
		enAnim_GetUp,//�N���オ��
		enAnim_Num//�A�j���[�V�����̐�
	};

	//�v���C���[�̃A�j���[�V�����J�ڂɎg���p�����[�^�[
	struct PlayerAnimationParam
	{
		//TODO:���̏����ׂĂ�bool�l�͏d������true�ɂȂ邱�Ƃ��Ȃ��̂ŁA��ɂ܂Ƃ߂���
		bool isWalking = false;
		bool isRunning = false;
		bool isGuarding = false;
		bool isCutMode = false;
		bool isKnockDown = false;
		int downTime = 0;
		int comboNum = 0;
		int attackingTime = 0;
		Vector3 playerPosition = Vector3::Zero;
	};
}