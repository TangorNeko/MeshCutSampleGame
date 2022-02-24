#include "stdafx.h"
#include "Explosion.h"

namespace
{
	const char16_t* EFFECT_EXPLOSION_PATH = u"Assets/effect/Boom.efk";	//�����G�t�F�N�g�̃p�X
	const Vector3 EFFECT_EXPLOSION_SCALE = { 30.0f,30.0f,30.0f };		//�����G�t�F�N�g�̊g�嗦
}

namespace Game
{
	void Explosion::Update()
	{
		m_currentFrame++;

		//��������t���[�����ɒB�����甚���G�t�F�N�g�𐶐�
		if (m_currentFrame == m_explodeFrame)
		{
			//�G�t�F�N�g�𐶐�
			Effect* explosionEffect = NewGO<Effect>(Priority::High);
			explosionEffect->Init(EFFECT_EXPLOSION_PATH);
			explosionEffect->SetPosition(m_position);
			explosionEffect->SetScale(EFFECT_EXPLOSION_SCALE);
			explosionEffect->Play();

			//�C���X�^���X�̍폜
			DeleteGO(this);
		}
	}
}