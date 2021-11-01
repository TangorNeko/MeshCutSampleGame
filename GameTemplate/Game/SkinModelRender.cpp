#include "stdafx.h"
#include "SkinModelRender.h"

namespace Game
{
	void SkinModelRender::Render(RenderContext& rc)
	{
		m_model->Draw(rc);
	}

	void SkinModelRender::Init(const char* modelPath, const char* skeletonPath, AnimationClip* animClips, int animClipNum, EnModelUpAxis modelUpAxis)
	{
		//���f�����Ȃ���ΐ���
		if (m_model == nullptr)
		{
			m_model = new Model;
		}

		//���f���̃t�@�C���p�X�̎w��
		m_modelInitData.m_tkmFilePath = modelPath;

		//�V�F�[�_�[�p�X�̎w��
		m_modelInitData.m_fxFilePath = "Assets/shader/model.fx";

		//�V�F�[�_�[�̒��_�V�F�[�_�[�̃G���g���[�֐����̎w��
		m_modelInitData.m_vsEntryPointFunc = "VSMain";

		//�V�F�[�_�[�̃s�N�Z���V�F�[�_�[�̃G���g���[�֐����̎w��
		m_modelInitData.m_vsSkinEntryPointFunc = "VSSkinMain";

		//�X�P���g�������݂��Ă��鎞�̓X�P���g����������
		if (skeletonPath != nullptr)
		{
			m_skeleton.Init(skeletonPath);
			m_modelInitData.m_skeleton = &m_skeleton;
		}

		//���f���f�[�^�̏�����̎����w��
		m_modelInitData.m_modelUpAxis = modelUpAxis;

		//���f���̏�����
		m_model->Init(m_modelInitData);

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

		//TODO:false�ɂ������ɐؒf�s�ɂ����鏈���A���������ؒf�\�ɂ�����ɕs�ɖ߂��悤�ȏ󋵂�����̂�
	}

	void SkinModelRender::Divide(const Vector3& cutNormal, const Vector3& cutPoint)
	{
		if (m_isDividable == true)
		{
			//���f���𕪊����A�\�ʑ��̃��f��������Ɋi�[�A���ʑ��̃��f�����|�C���^�Ƃ��Ď擾
			Model* backModel = m_model->Divide(m_modelInitData, cutNormal, cutPoint);

			//���S��1�����b�V���A�}�e���A�����c��Ȃ��������̏���
			if (backModel->GetTkmFile().GetNumMesh() == 0)
			{
				delete backModel;
				return;
			}
			
			//���ʑ��̃��f����`�悷�郂�f�������_�[�N���X���쐬
			SkinModelRender* backModelRender = NewGO<SkinModelRender>(0);

			//�V������������f���̏�����
			//TODO:�܂Ƃ߂�
			backModelRender->InitFromModel(backModel);
			backModelRender->SetPosition(m_position);
			backModelRender->SetRotation(m_qRot);
			backModelRender->SetScale(m_scale);
			backModelRender->SetDivideFlag(true);
			backModelRender->SetModelInitData(m_modelInitData);

			backModelRender->SetModelCenterAsOrigin();

			ModelCutManager::GetInstance()->AddNextCuttable(backModelRender);
			
		}
	}

	void SkinModelRender::SetModelCenterAsOrigin()
	{
		Vector3 originToCenter = m_model->GetOriginToCenter();
		m_model->SetOriginOffset(originToCenter, m_modelInitData);

		Vector3 worldOrigin = originToCenter;
		m_model->GetWorldMatrix().Apply(worldOrigin);
		SetPosition(worldOrigin);
	}
}