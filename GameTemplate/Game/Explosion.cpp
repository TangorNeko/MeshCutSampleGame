#include "stdafx.h"
#include "Explosion.h"

namespace
{
	const char16_t* EFFECT_EXPLOSION_PATH = u"Assets/effect/Boom.efk";	//爆発エフェクトのパス
	const Vector3 EFFECT_EXPLOSION_SCALE = { 30.0f,30.0f,30.0f };		//爆発エフェクトの拡大率
}

namespace Game
{
	void Explosion::Update()
	{
		m_currentFrame++;

		//爆発するフレーム数に達したら爆発エフェクトを生成
		if (m_currentFrame == m_explodeFrame)
		{
			//エフェクトを生成
			Effect* explosionEffect = NewGO<Effect>(Priority::High);
			explosionEffect->Init(EFFECT_EXPLOSION_PATH);
			explosionEffect->SetPosition(m_position);
			explosionEffect->SetScale(EFFECT_EXPLOSION_SCALE);
			explosionEffect->Play();

			//インスタンスの削除
			DeleteGO(this);
		}
	}
}