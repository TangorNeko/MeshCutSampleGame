#pragma once

namespace Game
{
	class PlayerAnimation
	{
	public:
		void InitAnim();

		void Update(SkinModelRender* playerRender);

		AnimationClip* GetAnimationClip()
		{
			return m_animationClips;
		}

		constexpr int GetAnimClipNum()
		{
			return static_cast<int>(enAnim_Num);
		}
	private:
		//�v���C���[�̃A�j���[�V�����ꗗ
		enum
		{
			enAnim_Idle,//�ҋ@
			enAnim_Walk,//���s
			enAnim_Attack,//�U��
			enAnim_Num//�A�j���[�V�����̐�
		};
		AnimationClip m_animationClips[enAnim_Num];	//�v���C���[�̃A�j���[�V�����N���b�v

		Vector3 m_prevPosition = Vector3::Zero;//�O�t���[���̃v���C���[�̈ʒu
	};
}

