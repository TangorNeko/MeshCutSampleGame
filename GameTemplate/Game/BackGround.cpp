#include "stdafx.h"
#include "BackGround.h"
#include "Door.h"
#include "TitleDoor.h"

namespace
{
	const char* PATH_STAGEMODEL = "Assets/modelData/Stage.tkm";		//�X�e�[�W���f���̃p�X
	const Vector3 STAGE_LIGHT_COLOR = Vector3::One;					//�X�e�[�W�̌����̐F
	const Vector3 STAGE_LIGHT_DIRECTION = { -1.0f,-1.0f,1.0f };		//�X�e�[�W�̕��s�����̌���
	const Vector3 LIGHTCAMERA_POSITION = { 500.0f,500.0f,-500.0f };	//���C�g�J�����̍��W
	const Vector3 LIGHTCAMERA_TARGET = { 0.0f,0.0f,0.0f };			//���C�g�J�����̒����_
	const Vector3 LIGHTCAMERA_UP = { 1.0f,0.0f,0.0f };				//���C�g�J�����̏����
	const float LIGHTCAMERA_WIDTH = 2000.0f;						//���C�g�J�������ʂ���(���s�����Ȃ̂ŃJ�����͕��s���e)
	const float LIGHTCAMERA_HEIGHT = 2000.0f;						//���C�g�J�������ʂ�����
}

namespace Game
{
	BackGround::~BackGround()
	{
		//���f���ƌ������폜
		DeleteGO(m_stageModel);
		DeleteGO(m_stageLight);
		DeleteGO(m_stageDoor);
	}

	bool BackGround::Start()
	{
		//�^�C�g���p�̃h�A���쐬
		NewGO<TitleDoor>(Priority::High, "titledoor");

		//�X�e�[�W�̃��f�����쐬
		m_stageModel = NewGO<SkinModelRender>(Priority::High);
		m_stageModel->Init(PATH_STAGEMODEL);
		m_stageModel->SetShadowCasterFlag(false);

		//�X�e�[�W�̌������쐬
		m_stageLight = NewGO<Light::DirectionLight>(Priority::High);
		m_stageLight->SetColor(STAGE_LIGHT_COLOR);
		m_stageLight->SetDirection(STAGE_LIGHT_DIRECTION);

		//�V���h�E�p���C�g�J�����̐ݒ�
		Light::LightManager::GetInstance()->SetLightCameraPosition(LIGHTCAMERA_POSITION);
		Light::LightManager::GetInstance()->SetLightCameraTarget(LIGHTCAMERA_TARGET);
		Light::LightManager::GetInstance()->SetLightCameraUp(LIGHTCAMERA_UP);
		Light::LightManager::GetInstance()->SetLightCameraUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
		Light::LightManager::GetInstance()->SetLightCameraWidth(LIGHTCAMERA_WIDTH);
		Light::LightManager::GetInstance()->SetLightCameraHeight(LIGHTCAMERA_HEIGHT);

		//�X�e�[�W�̃h�A���쐬
		m_stageDoor = NewGO<Door>(Priority::High, "door");

		//�X�e�[�W�̓����蔻����쐬
		m_physicsStaticObject.CreateFromModel(m_stageModel->GetModel(),m_stageModel->GetModel()->GetWorldMatrix());

		return true;
	}

	bool BackGround::isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint) const
	{
		//���f���ɐ������q�b�g���Ă��邩�𒲂ׂ�
		return m_stageModel->isLineHitModel(start, end, crossPoint);
	}
}