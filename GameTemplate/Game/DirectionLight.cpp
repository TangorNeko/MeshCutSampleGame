#include "stdafx.h"
#include "DirectionLight.h"

namespace Light
{
	//�f�B���N�V�������C�g�̐������炵����
	DirectionLight::~DirectionLight()
	{
		LightManager::GetInstance()->RemoveDirectionLight(m_dirLigTag);
	}

	//�f�B���N�V�������C�g�̐��𑝂₵����
	bool DirectionLight::Start()
	{
		m_dirLigTag = LightManager::GetInstance()->AddDirectionLight(GetLigData());

		return true;
	}

	//���t���[���J�����̈ʒu�ƃf�B���N�V�������C�g�̃f�[�^���X�V
	void DirectionLight::Update()
	{
		LightManager::GetInstance()->UpdateDirectionLight(m_dirLigTag, GetLigData());
	}
}