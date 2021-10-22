#include "stdafx.h"
#include "SkinModelRender.h"

namespace Game
{
	void SkinModelRender::Render(RenderContext& rc)
	{
		m_model->Draw(rc);
	}

	void SkinModelRender::Init(const char* modelPath, const char* skeletonPath, AnimationClip* animClips, int animClipNum)
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
		m_modelInitData.m_modelUpAxis = enModelUpAxisZ;

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

	void SkinModelRender::Init(const char* modelPath, const char* skeletonPath)
	{
		Init(modelPath, skeletonPath, nullptr, 0);
	}

	void SkinModelRender::Init(const char* modelPath)
	{
		Init(modelPath, nullptr, nullptr, 0);
	}

	void SkinModelRender::InitFromModel(Model* model)
	{
		if (m_model == nullptr)
		{
			m_model = model;
		}
	}

	void SkinModelRender::Divide(const Vector3& cutNormal, const Vector3& cutPoint)
	{
		if (m_isDividable == true)
		{
			//���f���𕪊����A�\�ʑ��̃��f��������Ɋi�[�A���ʑ��̃��f�����|�C���^�Ƃ��Ď擾
			Model* backModel = m_model->Divide(m_modelInitData, cutNormal, cutPoint);

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
		}
	}
}