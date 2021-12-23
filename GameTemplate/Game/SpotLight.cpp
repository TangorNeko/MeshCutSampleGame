#include "stdafx.h"
#include "SpotLight.h"

namespace Light
{
	//�X�|�b�g���C�g�̐������炵����
	SpotLight::~SpotLight()
	{
		LightManager::GetInstance()->RemoveSpotLight(m_spotLigTag);
	}

	//�X�|�b�g���C�g�̐��𑝂₵����
	bool SpotLight::Start()
	{
		m_spotLigTag = LightManager::GetInstance()->AddSpotLight(GetLigData());

		return true;
	}

	//���t���[���J�����̈ʒu�ƃX�|�b�g���C�g�̃f�[�^���X�V
	void SpotLight::Update()
	{
		LightManager::GetInstance()->UpdateSpotLight(m_spotLigTag, GetLigData());
	}
}