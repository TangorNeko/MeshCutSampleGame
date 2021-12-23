#include "stdafx.h"
#include "SpotLight.h"

namespace Light
{
	//スポットライトの数を減らした時
	SpotLight::~SpotLight()
	{
		LightManager::GetInstance()->RemoveSpotLight(m_spotLigTag);
	}

	//スポットライトの数を増やした時
	bool SpotLight::Start()
	{
		m_spotLigTag = LightManager::GetInstance()->AddSpotLight(GetLigData());

		return true;
	}

	//毎フレームカメラの位置とスポットライトのデータを更新
	void SpotLight::Update()
	{
		LightManager::GetInstance()->UpdateSpotLight(m_spotLigTag, GetLigData());
	}
}