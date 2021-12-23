#include "stdafx.h"
#include "PointLight.h"

namespace Light
{
	//ポイントライトの数を減らした時
	PointLight::~PointLight()
	{
		LightManager::GetInstance()->RemovePointLight(m_pointLigTag);
	}

	//ポイントライトの数を増やした時
	bool PointLight::Start()
	{
		m_pointLigTag = LightManager::GetInstance()->AddPointLight(GetLigData());

		return true;
	}

	//毎フレームカメラの位置とポイントライトのデータを更新
	void PointLight::Update()
	{
		LightManager::GetInstance()->UpdatePointLight(m_pointLigTag, GetLigData());
	}
}