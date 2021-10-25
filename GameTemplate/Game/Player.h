#pragma once
#include "PlayerMove.h"
#include "PlayerCamera.h"
#include "PlayerAnimation.h"
#include "PlayerAttack.h"
#include "PlayerCut.h"

namespace Game
{
	class Player : public IGameObject
	{
		~Player();

		bool Start() override;

		void Update() override;

	private:
		SkinModelRender* m_playerModel = nullptr;
		CharacterController m_charaCon;
		Vector3 m_position = Vector3::Zero;
		PlayerMove m_playerMove;
		PlayerCamera m_playerCamera;
		PlayerAnimation m_playerAnimation;
		PlayerAttack m_playerAttack;
		PlayerCut m_playerCut;
	};
}

