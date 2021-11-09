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
		//プレイヤーのアニメーション一覧
		enum
		{
			enAnim_Idle,//待機
			enAnim_Walk,//歩行
			enAnim_Attack,//攻撃
			enAnim_Num//アニメーションの数
		};
		AnimationClip m_animationClips[enAnim_Num];	//プレイヤーのアニメーションクリップ

		Vector3 m_prevPosition = Vector3::Zero;//前フレームのプレイヤーの位置
	};
}

