#include "stdafx.h"
#include "PointLight.h"

namespace Light
{
	//�|�C���g���C�g�̐������炵����
	PointLight::~PointLight()
	{
		LightManager::GetInstance()->RemovePointLight(m_pointLigTag);
	}

	//�|�C���g���C�g�̐��𑝂₵����
	bool PointLight::Start()
	{
		m_pointLigTag = LightManager::GetInstance()->AddPointLight(GetLigData());

		return true;
	}

	//���t���[���J�����̈ʒu�ƃ|�C���g���C�g�̃f�[�^���X�V
	void PointLight::Update()
	{
		LightManager::GetInstance()->UpdatePointLight(m_pointLigTag, GetLigData());
	}
}