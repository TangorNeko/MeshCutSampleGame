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

		//��������t���[�����ɒB�����甚���G�t�F�N�g�𐶐�
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