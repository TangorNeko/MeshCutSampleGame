#pragma once
#include "PlayerMove.h"
#include "PlayerCamera.h"
#include "PlayerAnimation.h"
#include "PlayerAttack.h"
#include "PlayerCut.h"
#include "PlayerDisplay.h"

namespace Game
{
	class Player : public IGameObject
	{
		~Player();

		bool Start() override;

		void Update() override;

	public:

		void SetPosition(const Vector3& position)
		{
			m_position = position;
			m_playerMove.GetCharaCon().SetPosition(m_position);
		}

		const Vector3& GetPosition() const
		{
			return m_position;
		}

		CharacterController& GetCharaCon()
		{
			return m_playerMove.GetCharaCon();
		}

		void Damage(int damage);

		void KnockDown(const Vector3 moveAmount)
		{
			m_playerMove.KnockDown(moveAmount);
		}

		//TODO:仮です。ガードは専用関数を作る、
		//アニメーションパラメーターは直接使いたくない。
		bool isGuard()
		{
			return m_playerAnimationParam.isGuarding;
		}
	private:
		SkinModelRender* m_playerModel = nullptr;	//プレイヤーのモデル
		Vector3 m_position = Vector3::Zero;			//プレイヤーの座標
		PlayerMove m_playerMove;					//プレイヤーの移動クラス
		PlayerCamera m_playerCamera;				//プレイヤーのカメラクラス
		PlayerAnimation m_playerAnimation;			//プレイヤーのアニメーションクラス
		PlayerAnimationParam m_playerAnimationParam;//プレイヤーのアニメーション制御ステータス
		PlayerAttack m_playerAttack;				//プレイヤーの攻撃クラス
		PlayerCut m_playerCut;						//プレイヤーの切断クラス
		PlayerDisplay m_playerDisplay;				//プレイヤーの情報表示クラス
		int m_hp = 1000;
		FontRender* m_hpRender = nullptr;
	};
}

