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

		const Vector3& GetCameraPosition()
		{
			return m_playerCamera.GetCameraPosition();
		}
		
		const Vector3& GetCameraTarget()
		{
			return m_playerCamera.GetCameraTarget();
		}

		void Damage(int damage);

		void Heal(int healValue);

		void KnockDown(const Vector3& moveAmount)
		{
			m_playerMove.KnockDown(moveAmount);
		}

		void BackHandSpring(const Vector3& moveAmount)
		{
			m_playerMove.BackHandSpring(moveAmount);
		}

		void NoticeMissileMoveEnd()
		{
			m_playerMove.NoticeMissileMoveEnd();
			m_eventCut = false;
		}

		void NoticeFrontMoveEnd()
		{
			m_playerMove.NoticeFrontMoveEnd();
			m_eventCut = false;
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
		bool m_eventCut = false;
		FontRender* m_hpRender = nullptr;
	};
}

