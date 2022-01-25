#include "stdafx.h"
#include "BackGround.h"
#include "Door.h"

namespace
{
	const char* PATH_STAGEMODEL = "Assets/modelData/TestAttachedStage.tkm";
	const Vector3 STAGE_LIGHT_COLOR = Vector3::One;
	const Vector3 STAGE_LIGHT_DIRECTION = { -1.0f,-1.0f,1.0f };

	const Vector3 LIGHTCAMERA_POSITION = { 500.0f,500.0f,-500.0f };	//ライトカメラの座標
	const Vector3 LIGHTCAMERA_TARGET = { 0.0f,0.0f,0.0f };			//ライトカメラの注視点
	const Vector3 LIGHTCAMERA_UP = { 1.0f,0.0f,0.0f };				//ライトカメラの上方向
	const float LIGHTCAMERA_WIDTH = 2000.0f;						//ライトカメラが写す幅(平行光源なのでカメラは平行投影)
	const float LIGHTCAMERA_HEIGHT = 2000.0f;						//ライトカメラが写す高さ
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
		m_stageModel->SetShadowCasterFlag(false);

		m_stageLight = NewGO<Light::DirectionLight>(0);
		m_stageLight->SetColor(STAGE_LIGHT_COLOR);
		m_stageLight->SetDirection(STAGE_LIGHT_DIRECTION);

		//シャドウ用ライトカメラの設定
		Light::LightManager::GetInstance()->SetLightCameraPosition(LIGHTCAMERA_POSITION);
		Light::LightManager::GetInstance()->SetLightCameraTarget(LIGHTCAMERA_TARGET);
		Light::LightManager::GetInstance()->SetLightCameraUp(LIGHTCAMERA_UP);
		Light::LightManager::GetInstance()->SetLightCameraUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
		Light::LightManager::GetInstance()->SetLightCameraWidth(LIGHTCAMERA_WIDTH);
		Light::LightManager::GetInstance()->SetLightCameraHeight(LIGHTCAMERA_HEIGHT);

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