#include "stdafx.h"
#include "StepObject.h"

namespace
{
	const float MOVESPEED = 10.0f;
	const int MAX_LIFETIME = 400;
}

namespace Game
{
	StepObject::~StepObject()
	{
		DeleteGO(m_skinModelRender);
	}

	bool StepObject::Start()
	{
		m_skinModelRender = NewGO<SkinModelRender>(0);
		m_skinModelRender->Init("Assets/modelData/TankMissile.tkm");

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

		//�������Ԃ��C���N�������g
		m_lifeTime++;

		//�ő吶�����ԂɂȂ�����폜
		if (m_lifeTime == MAX_LIFETIME)
		{
			DeleteGO(this);
		}
	}
}
