#include "stdafx.h"
#include "BackGround.h"
#include "Door.h"

namespace
{
	const char* PATH_STAGEMODEL = "Assets/modelData/Stagetest.tkm";
	const Vector3 STAGE_LIGHT_COLOR = Vector3::One;
	const Vector3 STAGE_LIGHT_DIRECTION = Vector3::Down;
}

namespace Game
{
	BackGround::~BackGround()
	{
		DeleteGO(m_stageModel);
		DeleteGO(m_stageLight);
		DeleteGO(m_stageDoor);
	}

	bool BackGround::Start()
	{
		m_stageModel = NewGO<SkinModelRender>(0);
		m_stageModel->Init(PATH_STAGEMODEL);

		m_stageLight = NewGO<Light::DirectionLight>(0);
		m_stageLight->SetColor(STAGE_LIGHT_COLOR);
		m_stageLight->SetDirection(STAGE_LIGHT_DIRECTION);

		m_stageDoor = NewGO<Door>(0, "door");

		m_physicsStaticObject.CreateFromModel(m_stageModel->GetModel(),m_stageModel->GetModel()->GetWorldMatrix());

		return true;
	}

	bool BackGround::isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint) const
	{
		return m_stageModel->isLineHitModel(start, end, crossPoint);
	}

	void BackGround::Update()
	{

	}
}