#include "stdafx.h"
#include "PlayerAttack.h"
#include "Enemy.h"

namespace
{
	const char* PATH_HITBOXMODEL = "Assets/modelData/ball.tkm";
	const float TIME_ATTACK_END = 60.0f;
	const float ATTACK_RANGE = 200.0f;
	const float ATTACK_DAMAGE = 25.0f;
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
		//攻撃中ならモデルを追従してタイマーに加算
		if (m_isAttacking == true)
		{
			m_testHitBox->SetPosition(playerPosition);
			m_attackTime++;
		}

		//攻撃中でない時にXボタンを押すと攻撃
		if (g_pad[0]->IsTrigger(enButtonX) && m_isAttacking == false)
		{
			m_testHitBox = NewGO<SkinModelRender>(0);
			m_testHitBox->Init(PATH_HITBOXMODEL);
			m_testHitBox->SetPosition(playerPosition);
			m_isAttacking = true;

			//近くの敵にダメージを与える
			QueryGOs<Enemy>("enemy", [&playerPosition](Enemy* enemy)
				{
					Vector3 distance = playerPosition - enemy->GetPosition();

					if (distance.LengthSq() < ATTACK_RANGE * ATTACK_RANGE)
					{
						enemy->Damage(ATTACK_DAMAGE);
					}
					return true;
				}
			);
		}

		//攻撃後60フレーム経つとモデル削除
		if (m_attackTime > 60.0f)
		{
			DeleteGO(m_testHitBox);
			m_testHitBox = nullptr;
			m_isAttacking = false;
			m_attackTime = 0.0f;
		}
	}
}