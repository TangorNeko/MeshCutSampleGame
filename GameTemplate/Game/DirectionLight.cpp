#include "stdafx.h"
#include "DirectionLight.h"

namespace Light
{
	//ディレクションライトの数を減らした時
	DirectionLight::~DirectionLight()
	{
		LightManager::GetInstance()->RemoveDirectionLight(m_dirLigTag);
	}

	//ディレクションライトの数を増やした時
	bool DirectionLight::Start()
	{
		m_dirLigTag = LightManager::GetInstance()->AddDirectionLight(GetLigData());

		return true;
	}

	//毎フレームカメラの位置とディレクションライトのデータを更新
	void DirectionLight::Update()
	{
		LightManager::GetInstance()->UpdateDirectionLight(m_dirLigTag, GetLigData());
	}
}