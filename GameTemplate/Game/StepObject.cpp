#include "stdafx.h"
#include "StepObject.h"

namespace
{
	const float MOVESPEED = 10.0f;
	const int MAX_LIFETIME = 400;
	const char16_t* EFFECT_MISSILE_PATH = u"Assets/effect/MissileTail.efk";
	const Vector3 EFFECT_MISSILE_SCALE = { 25.0f,25.0f,25.0f };
	const float EFFECT_MISSILE_ROTATEDEG = -90.0f;
}

namespace Game
{
	StepObject::~StepObject()
	{
		DeleteGO(m_skinModelRender);

		m_missileEffect->Stop();
		DeleteGO(m_missileEffect);
	}

	bool StepObject::Start()
	{
		m_skinModelRender = NewGO<SkinModelRender>(0);
		m_skinModelRender->Init("Assets/modelData/Missile_Flat.tkm");

		m_missileEffect = NewGO<Effect>(0);
		m_missileEffect->Init(EFFECT_MISSILE_PATH);
		m_missileEffect->SetScale(EFFECT_MISSILE_SCALE);
		m_missileEffect->Play();

		return true;
	}

	void StepObject::Update()
	{
		//�ړ������ւ̉�]���v�Z
		m_qRot.SetRotation(Vector3::Up, m_moveDirection);
		
		//�ړ������ֈړ�
		m_position += m_moveDirection * MOVESPEED;

		//���W�Ɖ�]���Z�b�g
		m_skinModelRender->SetPosition(m_position);
		m_skinModelRender->SetRotation(m_qRot);

		//�~�T�C���̊p�x���Z�b�g
		Quaternion MissileRot;
		MissileRot.SetRotationDegX(EFFECT_MISSILE_ROTATEDEG);
		MissileRot.Multiply(m_qRot);
		m_missileEffect->SetPosition(m_position);
		m_missileEffect->SetRotation(MissileRot);

		//�������Ԃ��C���N�������g
		m_lifeTime++;

		//�ő吶�����ԂɂȂ�����폜
		if (m_lifeTime == MAX_LIFETIME)
		{
			DeleteGO(this);
		}
	}
}
