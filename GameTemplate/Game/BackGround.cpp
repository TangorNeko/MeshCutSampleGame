#include "stdafx.h"
#include "BackGround.h"

namespace
{
	const char* PATH_STAGEMODEL = "Assets/modelData/Stagetest.tkm";
	const Vector3 STAGE_POSITION = Vector3::Zero;
	const Vector3 STAGE_SCALE = { 10.0f,10.0f,10.0f };
}

namespace Game
{
	BackGround::~BackGround()
	{
		DeleteGO(m_stageModel);
		DeleteGO(m_stageLight);
		DeleteGO(m_doorModel);
	}

	bool BackGround::Start()
	{
		m_stageModel = NewGO<SkinModelRender>(0);
		m_stageModel->Init(PATH_STAGEMODEL);
		m_stageModel->SetPosition(STAGE_POSITION);
		m_stageModel->SetScale(Vector3::One);

		m_stageLight = NewGO<Light::DirectionLight>(0);
		m_stageLight->SetColor(Vector3::One);
		m_stageLight->SetDirection(Vector3::Down);

		m_doorModel = NewGO<SkinModelRender>(0);
		m_doorModel->Init("Assets/modelData/Door.tkm");
		m_doorModel->SetPosition(Vector3::Zero);
		m_doorModel->SetScale(Vector3::One);
		m_doorModel->SetDivideFlag(true);
		m_doorModel->SetModelCenterAsOrigin();
		ModelCutManager::GetInstance()->AddCuttable(m_doorModel);

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