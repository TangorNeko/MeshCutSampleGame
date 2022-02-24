#pragma once
#include "PlayerAnimationParam.h"

namespace Game
{
	class PlayerAttack
	{
	public:
		/**
		 * @brief プレイヤーの攻撃処理の更新
		 * @param playerPosition プレイヤーの座標
		 * @param animParam プレイヤーのアニメーションパラメーター
		 * @param toMoveRot 移動方向へのクォータニオン
		*/
		void Update(const Vector3& playerPosition,PlayerAnimationParam& animParam,const Quaternion& toMoveRot);
	private:
		int m_attackTime = 0;			//攻撃時間
		int m_comboNum = 0;				//攻撃段数(0:攻撃していない　1:コンボ1　2:コンボ2　3:コンボ3)
	};
}

