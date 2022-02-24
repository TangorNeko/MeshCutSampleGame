#include "stdafx.h"
#include "Explosion.h"

namespace
{
	const Vector3 EFFECT_EXPLOSION_SCALE = { 30.0f,30.0f,30.0f };
}

namespace Game
{
	Explosion::~Explosion()
	{

	}

	bool Explosion::Start()
	{
		return true;
	}

	void Explosion::Update()
	{
		m_currentFrame++;

		//爆発するフレーム数に達したら爆発エフェクトを生成
		if (m_currentFrame == m_explodeFrame)
		{
			Effect* explosionEffect = NewGO<Effect>(Priority::High);
			explosionEffect->Init(u"Assets/effect/Boom.efk");
			explosionEffect->SetPosition(m_position);
			explosionEffect->SetScale(EFFECT_EXPLOSION_SCALE);
			explosionEffect->Play();

			DeleteGO(this);
		}
	}
}