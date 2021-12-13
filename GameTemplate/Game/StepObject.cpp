#include "stdafx.h"
#include "StepObject.h"

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
		Quaternion qRot;
		qRot.SetRotationDegX(90.0f);
		m_skinModelRender->SetRotation(qRot);

		return true;
	}

	void StepObject::Update()
	{
		m_position.z += 5.0f;
		m_skinModelRender->SetPosition(m_position);
	}
}
