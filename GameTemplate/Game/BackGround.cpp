#include "stdafx.h"
#include "BackGround.h"

namespace
{
	const char* PATH_STAGEMODEL = "Assets/modelData/ProtoStage.tkm";
	const Vector3 STAGE_POSITION = Vector3::Zero;
	const Vector3 STAGE_SCALE = { 1.0f,1.0f,1.0f };
}

namespace Game
{
	BackGround::~BackGround()
	{
		DeleteGO(m_stageModel);
		DeleteGO(m_stageLight);
	}

	bool BackGround::Start()
	{
		m_stageModel = NewGO<SkinModelRender>(0);
		m_stageModel->Init(PATH_STAGEMODEL);
		m_stageModel->SetPosition(STAGE_POSITION);
		m_stageModel->SetScale(STAGE_SCALE);

		m_stageLight = NewGO<Light::DirectionLight>(0);
		m_stageLight->SetColor(Vector3::One);
		m_stageLight->SetDirection(Vector3::Down);

		m_physicsStaticObject.CreateFromModel(m_stageModel->GetModel(),m_stageModel->GetModel()->GetWorldMatrix());

		return true;
	}

	void BackGround::Update()
	{

	}
}