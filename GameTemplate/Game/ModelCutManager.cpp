#include "stdafx.h"
#include "ModelCutManager.h"

namespace Game
{
	ModelCutManager* ModelCutManager::m_instance = nullptr;

	void ModelCutManager::AddCuttable(SkinModelRender* cuttableModel)
	{
		//�ؒf�t���O��true�Ȃ�
		if (cuttableModel->GetDivideFlag() == true)
		{
			//�J�b�g���郂�f���̃R���e�i�ɑ}��
			m_cutModelPtrSet.insert(cuttableModel);
		}
	}

	void ModelCutManager::QueryCut(const Vector3& cutNormal, const Vector3& cutPoint, std::function<bool(const SkinModelRender* modelRender)> cutJudgeFunc)
	{
		//�ؒf���f���̃��X�g�𑖍�
		for (auto* cutObject : m_cutModelPtrSet)
		{
			//�ؒf���邩�̔���֐������s
			bool divideJudge = cutJudgeFunc(cutObject);

			//�ؒf����Ɣ��肳�ꂽ��ؒf
			if (divideJudge)
			{
				cutObject->Divide(cutNormal, cutPoint);
			}
		}

		//�ؒf�̌��ʐV�����ł����ؒf�\���f�������X�g�ɐς�ł���
		for (auto* addObject : m_nextCutModelPtrSet)
		{
			m_cutModelPtrSet.insert(addObject);
		}

		//�V�����ł����ؒf�\���f���̃��X�g���폜
		m_nextCutModelPtrSet.clear();
	}

	void ModelCutManager::QueryCut(const Vector3& cutNormal, std::function<bool(const SkinModelRender* modelRender)> cutJudgeFunc)
	{
		//�ؒf���f���̃��X�g�𑖍�
		for (auto* cutObject : m_cutModelPtrSet)
		{
			//�ؒf���邩�̔���֐������s
			bool divideJudge = cutJudgeFunc(cutObject);

			//�ؒf����Ɣ��肳�ꂽ��ؒf
			if (divideJudge)
			{
				//�ؒf���_����ؒf
				cutObject->Divide(cutNormal, cutObject->GetPosition());
			}
		}

		//�ؒf�̌��ʐV�����ł����ؒf�\���f�������X�g�ɐς�ł���
		for (auto* addObject : m_nextCutModelPtrSet)
		{
			m_cutModelPtrSet.insert(addObject);
		}

		//�V�����ł����ؒf�\���f���̃��X�g���폜
		m_nextCutModelPtrSet.clear();
	}
}