#include "stdafx.h"
#include "SkinModelRender.h"
#include "CutDummy.h"

namespace
{
	const int MODE_MAX_DIVIDE_NUM = 3;
}

namespace Game
{
	void SkinModelRender::Render(RenderContext& rc,Camera* camera)
	{
		switch (rc.GetRenderStep())
		{
		case RenderContext::enModel:
			m_model->Draw(rc,camera);
			break;
		case RenderContext::enShadow:
			if (m_shadowModel != nullptr && m_shadowCasterFlag == true)
			{
				m_shadowModel->Draw(rc, camera);
			}
		}
	}

	void SkinModelRender::Init(const char* modelPath, const char* skeletonPath, AnimationClip* animClips, int animClipNum, EnModelUpAxis modelUpAxis)
	{
		//���f�����Ȃ���ΐ���
		if (m_model == nullptr)
		{
			m_model = new Model;
		}

		if (m_shadowModel == nullptr)
		{
			m_shadowModel = new Model;
		}

		//���f���̃t�@�C���p�X�̎w��
		m_modelInitData.m_tkmFilePath = modelPath;

		//�V�F�[�_�[�p�X�̎w��
		m_modelInitData.m_fxFilePath = "Assets/shader/shadowReceiver.fx";

		//�V�F�[�_�[�̒��_�V�F�[�_�[�̃G���g���[�֐����̎w��
		m_modelInitData.m_vsEntryPointFunc = "VSMain";

		//�V�F�[�_�[�̃X�L�����蒸�_�V�F�[�_�[�̃G���g���[�֐����̎w��
		m_modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";

		//�X�P���g�������݂��Ă��鎞�̓X�P���g����������
		if (skeletonPath != nullptr)
		{
			m_skeleton.Init(skeletonPath);
			m_modelInitData.m_skeleton = &m_skeleton;
		}

		//���f���f�[�^�̏�����̎����w��
		m_modelInitData.m_modelUpAxis = modelUpAxis;

		//��������͉e�p��ModelInitData�ƒʏ탂�f���p��ModelInitData���Ⴄ�̂�2�ɕ�����B
		ModelInitData shadowModelInitData = m_modelInitData;

		m_modelInitData.m_expandShaderResoruceView = &Graphics::g_renderingEngine->GetShadowMap();

		m_modelInitData.m_expandConstantBufferSize[0] = Light::LightManager::GetInstance()->GetLigDataSize();
		m_modelInitData.m_expandConstantBuffer[0] = Light::LightManager::GetInstance()->GetLigDatas();
		
		m_modelInitData.m_expandConstantBufferSize[1] = Light::LightManager::GetInstance()->GetLigCameraDataSize();
		m_modelInitData.m_expandConstantBuffer[1] = Light::LightManager::GetInstance()->GetLigCameraDatas();

		shadowModelInitData.m_fxFilePath = "Assets/shader/shadow.fx";
		shadowModelInitData.m_colorBufferFormat = DXGI_FORMAT_R32_FLOAT;
		shadowModelInitData.m_expandConstantBufferSize[0] = Light::LightManager::GetInstance()->GetLigCameraDataSize();
		shadowModelInitData.m_expandConstantBuffer[0] = Light::LightManager::GetInstance()->GetLigCameraDatas();

		//���f���̏�����
		m_model->Init(m_modelInitData);
		m_shadowModel->Init(shadowModelInitData);

		//�A�j���[�V�����֘A�̏�����
		m_animationClips = animClips;
		m_animationClipNum = animClipNum;

		if (m_animationClips != nullptr)
		{
			m_animation.Init(m_skeleton, m_animationClips, m_animationClipNum);
		}
	}

	void SkinModelRender::Init(const char* modelPath, const char* skeletonPath, EnModelUpAxis modelUpAxis)
	{
		Init(modelPath, skeletonPath, nullptr, 0, modelUpAxis);
	}

	void SkinModelRender::Init(const char* modelPath, EnModelUpAxis modelUpAxis)
	{
		Init(modelPath, nullptr, nullptr, 0, modelUpAxis);
	}

	void SkinModelRender::InitUnlit(const char* modelPath, EnModelUpAxis modelUpAxis)
	{
		//���f�����Ȃ���ΐ���
		if (m_model == nullptr)
		{
			m_model = new Model;
		}

		//���f���̃t�@�C���p�X�̎w��
		m_modelInitData.m_tkmFilePath = modelPath;

		//�V�F�[�_�[�p�X�̎w��
		m_modelInitData.m_fxFilePath = "Assets/shader/unlit.fx";

		//�V�F�[�_�[�̒��_�V�F�[�_�[�̃G���g���[�֐����̎w��
		m_modelInitData.m_vsEntryPointFunc = "VSMain";

		//�V�F�[�_�[�̃s�N�Z���V�F�[�_�[�̃G���g���[�֐����̎w��
		m_modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";

		//���f���f�[�^�̏�����̎����w��
		m_modelInitData.m_modelUpAxis = modelUpAxis;

		//���f���̏�����
		m_model->Init(m_modelInitData);
	}

	bool SkinModelRender::isLineHitModel(const Vector3& start, const Vector3& end, Vector3& crossPoint)
	{
		return m_model->isLineHitModel(start, end, crossPoint);
	}

	void SkinModelRender::InitFromModel(Model* model)
	{
		if (m_model == nullptr)
		{
			m_model = model;
		}
	}

	void SkinModelRender::SetDivideFlag(bool isDividable)
	{
		m_isDividable = isDividable;

		if (m_isDividable == false)
		{
			//�J�b�g�\���f������폜
			ModelCutManager::GetInstance()->RemoveCuttable(this);
		}
	}

	void SkinModelRender::Divide(const Vector3& cutNormal, const Vector3& cutPoint, const Vector3& cutForce)
	{
		//�ؒf�s�ȏ�ԂȂ牽�����Ȃ��B
		if (m_isDividable == false || m_divideNum > MODE_MAX_DIVIDE_NUM)
		{
			return;
		}

		Vector3 coreRemainCutNormal = cutNormal;
		//�R�A�̍��W���c���悤�ɔ��肵�K�v�Ȃ�@���𔽓]����
		if (IsCoreOnFront(coreRemainCutNormal, cutPoint) == false)
		{
			coreRemainCutNormal *= -1.0f;
		}

		//���f���𕪊����A�\�ʑ��̃��f��������Ɋi�[�A���ʑ��̃��f�����|�C���^�Ƃ��Ď擾
		Model* backModel = m_model->Divide(m_modelInitData, coreRemainCutNormal, cutPoint);

		//�ؒf����Őؒf����Ȃ��Ɣ��f���ꂽ�ꍇ�������Ȃ�
		if (backModel == nullptr)
		{
			return;
		}

		//���S��1�����b�V���A�}�e���A�����c��Ȃ������ꍇ�̏���
		//NOTE:�ؒf����Ȃ��Ɣ��f���ꂽ�ꍇ�ȊO�Ɋ��S�Ƀ��b�V���A�}�e���A�����c��Ȃ��ꍇ�����邩�͕s��
		if (backModel->GetTkmFile().GetNumMesh() == 0)
		{
			delete backModel;
			return;
		}
		
		//���f���ꂽ�̂Őؒf�񐔂��v���X
		m_divideNum++;

		//�\��(���̃��f��)�̉e�p���f�����č\��
		ShadowModelReconstruction();

		//���ʑ��̃��f����`�悷�郂�f�������_�[�N���X���쐬
		SkinModelRender* backModelRender = NewGO<SkinModelRender>(0);

		//�V������������f���̏�����
		//TODO:�܂Ƃ߂�
		backModelRender->InitFromModel(backModel);
		backModelRender->SetModelInitData(m_modelInitData);

		//����(�V�����ł������f��)�̉e�p���f�����\��
		//TODO:�����ɂ͗����̎��͍č\���ł͂Ȃ�
		backModelRender->ShadowModelReconstruction();

		backModelRender->SetPosition(m_position);
		backModelRender->SetRotation(m_qRot);
		backModelRender->SetScale(m_scale);
		backModelRender->SetDivideFlag(true);

		//�������ꂽ�񐔂����L
		backModelRender->SetDivideNum(m_divideNum);

		//�����G���W���𗘗p�����_�~�[�̍쐬

		backModelRender->MakeDummy(cutForce);

		//�J�b�g�\�ȃ��f���ꗗ�ɒǉ�
		ModelCutManager::GetInstance()->AddNextCuttable(backModelRender);

		if (m_owner != nullptr)
		{
			m_owner->OnDivide(this,cutForce);
		}
	}

	void SkinModelRender::MakeDummy(const Vector3& cutForce)
	{
		//�؂藣���ꂽ���f���̌��_��AABB���烂�f���̒��S�ɐݒ肵�A���S����AABB�܂ł̈�_�܂ł̋������擾
		SetModelCenterAsOrigin();

		//�J�v�Z���̃f�[�^���擾
		Vector2 heightAndRadius;
		Vector3 capsuleAxis = CalcCapsuleData(heightAndRadius);

		CutDummy* dummy = NewGO<CutDummy>(0,"cutDummy");
		dummy->SetSkinModel(this);

		//�J�v�Z���̃f�[�^���Z�b�g
		
		//heightAndRadius��x�ɍ����Ay�ɔ��a�������Ă���
		//NOTE:�ؒf����郂�f���̊g�嗦��xyz���ׂē���łȂ��Ƃ����������܂��a��Ȃ��̂ŁA
		//�����ꂩ�̊g�嗦����Z����΃��f���̑傫���ɍ������J�v�Z���̑傫���ɂȂ�
		dummy->SetCapsuleHeight(heightAndRadius.x * m_scale.x);
		dummy->SetCapsuleRadius(heightAndRadius.y * m_scale.x);

		//�J�v�Z���̎������[�J�����W�n�Ȃ̂Ń��[���h���W�n�ɕϊ�
		Matrix modelWorldMatrix = m_model->GetWorldMatrix();
		modelWorldMatrix.Apply(capsuleAxis);
		Vector3 localOrigin = Vector3::Zero;
		modelWorldMatrix.Apply(localOrigin);
		capsuleAxis -= localOrigin;
		capsuleAxis.Normalize();

		//�J�v�Z���͍ŏ��Ɏ����X���Ă��镪��]�����邪�A���̂܂܂��ƃ��f�������̉�]�ɂ��Ă���̂ŁA
		//�߂��N�H�[�^�j�I�������߂Ă���
		Quaternion capsuleRot, toModelRot;
		//�J�v�Z���̌X���̃N�H�[�^�j�I��
		capsuleRot.SetRotation(Vector3::Up, capsuleAxis);
		//�J�v�Z���̌X����߂�(���f���ɏ�Z����)�N�H�[�^�j�I��
		toModelRot.SetRotation(capsuleAxis, Vector3::Up);

		dummy->SetRotations(capsuleRot, toModelRot);
		dummy->SetCutForce(cutForce);
	}

	void SkinModelRender::SetModelCenterAsOrigin()
	{
		//���f���̌��݂̌��_���烂�f���̒��S�ւ̃x�N�g�������߂�
		Vector3 OriginOffset = m_model->GetOriginToCenter();

		//�x�N�g�������f���̌��_�����炷
		m_model->SetOriginOffset(OriginOffset, m_modelInitData);
		m_shadowModel->SetOriginOffset(OriginOffset, m_modelInitData);

		Vector3 worldOrigin = OriginOffset;

		//���̂܂܂��ƕ\���ʒu�������̂ł��炵������ł������悤�ɍ��W�ړ�
		m_model->GetWorldMatrix().Apply(worldOrigin);
		SetPosition(worldOrigin);
	}

	bool SkinModelRender::IsCoreOnFront(const Vector3& cutNormal, const Vector3& cutPoint)
	{
		//�ؒf�ʂ̈�_����R�A�ւ̃x�N�g���Ɩ@�������̃x�N�g�������߂Đ��K������
		Vector3 corePosition = m_position + m_toCoreVector;
		Vector3 pointToCore = corePosition - cutPoint;
		pointToCore.Normalize();

		//���ς����߂�
		float dot = Dot(pointToCore, cutNormal);

		//���ς̌��ʂ�0�ȏ�Ȃ�@���Ɠ��������@�܂�\�ɂ���
		if (dot >= 0)
		{
			return true;
		}
		else
		{
			//0�����Ȃ�@���ƈႤ�����@�܂藠�ɂ���
			return false;
		}
	}

	void SkinModelRender::ShadowModelReconstruction()
	{
		//���݂̉e���f�����폜
		delete m_shadowModel;

		//�V�����e���f�����쐬
		m_shadowModel = new Model;

		//���f������R�s�[���Ă���B
		m_model->CopyTo(m_shadowModel);

		//�e�p��ModelInitData�̐ݒ�
		ModelInitData shadowModelInitData = m_modelInitData;

		shadowModelInitData.m_fxFilePath = "Assets/shader/shadow.fx";
		shadowModelInitData.m_colorBufferFormat = DXGI_FORMAT_R32_FLOAT;
		shadowModelInitData.m_expandConstantBufferSize[0] = Light::LightManager::GetInstance()->GetLigCameraDataSize();
		shadowModelInitData.m_expandConstantBuffer[0] = Light::LightManager::GetInstance()->GetLigCameraDatas();

		m_shadowModel->TkmFileToMeshParts(shadowModelInitData);
	}
}