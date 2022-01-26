#pragma once

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
		enAnim_Jump,//�W�����v
		enAnim_Unequip,//�[��
		enAnim_Idle_Unequip,//�[����Ԃł̑ҋ@
		enAnim_BackHandspring,//��]
		enAnim_Num//�A�j���[�V�����̐�
	};

	//�v���C���[�̃A�j���[�V�����J�ڂɎg���p�����[�^�[
	struct PlayerAnimationParam
	{
		//�v���C���[�̏��
		enum PlayerState
		{
			enIdle,				//�ҋ@��
			enWalk,				//���s��
			enRunning,			//���蒆
			enGuard,			//�K�[�h��
			enCutMode,			//�ؒf���[�h��
			enKnockDown,		//�m�b�N�_�E����
			enJumping,			//�W�����v��
			enUnequip,			//�[����
			enBackHandSpring,	//��]��
		};

		PlayerState playerState = enIdle;	//���݂̃v���C���[�̏��
		bool isPlayingAnimation = false;	//�A�j���[�V�������Đ���?
		int downTime = 0;
		int comboNum = 0;
		int attackingTime = 0;
		int handspringTime = 0;
	};
}