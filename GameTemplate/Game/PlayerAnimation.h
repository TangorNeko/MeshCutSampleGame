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
		enum
		{
			enAnim_Idle,
			enAnim_Walk,
			enAnim_Num
		};
		AnimationClip m_animationClips[enAnim_Num];

		Vector3 m_prevPosition = Vector3::Zero;
	};
}

