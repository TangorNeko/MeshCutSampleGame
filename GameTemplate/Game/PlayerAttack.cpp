#include "stdafx.h"
#include "PlayerAttack.h"

namespace
{
	const char* PATH_HITBOXMODEL = "Assets/modelData/ball.tkm";
	const float TIME_ATTACK_END = 60.0f;
}

namespace Game
{
	PlayerAttack::~PlayerAttack()
	{
		if (m_testHitBox != nullptr)
		{
			DeleteGO(m_testHitBox);
		}
	}

	void PlayerAttack::Update(const Vector3& playerPosition)
	{
		//UŒ‚’†‚È‚çƒ‚ƒfƒ‹‚ð’Ç]‚µ‚Äƒ^ƒCƒ}[‚É‰ÁŽZ
		if (m_isAttacking == true)
		{
			m_testHitBox->SetPosition(playerPosition);
			m_attackTime++;
		}

		//UŒ‚’†‚Å‚È‚¢Žž‚ÉXƒ{ƒ^ƒ“‚ð‰Ÿ‚·‚ÆUŒ‚
		if (g_pad[0]->IsTrigger(enButtonX) && m_isAttacking == false)
		{
			m_testHitBox = NewGO<SkinModelRender>(0);
			m_testHitBox->Init(PATH_HITBOXMODEL);
			m_testHitBox->SetPosition(playerPosition);
			m_isAttacking = true;
		}

		//UŒ‚Œã60ƒtƒŒ[ƒ€Œo‚Â‚Æƒ‚ƒfƒ‹íœ
		if (m_attackTime > 60.0f)
		{
			DeleteGO(m_testHitBox);
			m_testHitBox = nullptr;
			m_isAttacking = false;
			m_attackTime = 0.0f;
		}
	}
}