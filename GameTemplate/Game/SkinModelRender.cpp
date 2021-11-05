#include "stdafx.h"
#include "SkinModelRender.h"
#include "CutDummy.h"

namespace
{
	const int MODE_MAX_DIVIDE_NUM = 4;
}

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
		if (m_isDividable == true && m_divideNum <= MODE_MAX_DIVIDE_NUM)
		{
			//���f���𕪊����A�\�ʑ��̃��f��������Ɋi�[�A���ʑ��̃��f�����|�C���^�Ƃ��Ď擾
			Model* backModel = m_model->Divide(m_modelInitData, cutNormal, cutPoint);

			//���S��1�����b�V���A�}�e���A�����c��Ȃ��������̏���
			if (backModel->GetTkmFile().GetNumMesh() == 0)
			{
				delete backModel;
				return;
			}
			
			//���f���ꂽ�̂Őؒf�񐔂��v���X
			m_divideNum++;

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

			//�������ꂽ�񐔂����L
			backModelRender->SetDivideNum(m_divideNum);

			//�����G���W���𗘗p�����_�~�[�̍쐬
			//NOTE:�J�b�g����郂�f���̖{�̂͂����ƕ����G���W�����g�p���Ȃ��`�Ŏc��܂��B

			//�؂藣���ꂽ���f���̌��_��AABB���烂�f���̒��S�ɐݒ肵�A���S����AABB�܂ł̈�_�܂ł̋������擾
			backModelRender->SetModelCenterAsOrigin();

			//�J�v�Z���̃f�[�^���擾
			Vector2 heightAndRadius;
			Vector3 capsuleAxis = backModelRender->CalcCapsuleData(heightAndRadius);

			CutDummy* dummy = NewGO<CutDummy>(0);
			dummy->SetSkinModel(backModelRender);

			//�J�v�Z���̃f�[�^���Z�b�g
			//TODO:��]���K�p
			//heightAndRadius��x�ɍ����Ay�ɔ��a�������Ă���
			dummy->SetCapsuleHeight(heightAndRadius.x);
			dummy->SetCapsuleRadius(heightAndRadius.y);

			//�J�b�g�\�ȃ��f���ꗗ�ɒǉ�
			ModelCutManager::GetInstance()->AddNextCuttable(backModelRender);
		}
	}

	void SkinModelRender::SetModelCenterAsOrigin()
	{
		Vector4 originToCenter = m_model->GetOriginToCenter();
		Vector3 OriginOffset = { originToCenter.x,originToCenter.y ,originToCenter.z };
		m_model->SetOriginOffset(OriginOffset, m_modelInitData);

		Vector3 worldOrigin = OriginOffset;

		//NOTE:���f�����g�傳��Ă���ꍇw�ɓ����Ă��鋗�����g�傳���̂ł͂Ȃ���?���̏ꍇ�͂ǂ��v�Z�����炢�����낤���B
		m_model->GetWorldMatrix().Apply(worldOrigin);
		SetPosition(worldOrigin);
	}
}