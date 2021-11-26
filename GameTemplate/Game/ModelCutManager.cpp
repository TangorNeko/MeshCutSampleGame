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

	void ModelCutManager::QueryCut(const Vector3& cutNormal, const Vector3& cutPoint, float cutDistance)
	{
		//�ؒf���f���̃��X�g�𑖍�
		for (auto* cutObject : m_cutModelPtrSet)
		{
			//�ؒf�ʂƂ̐ؒf���f���̋����𒲂ׂ�
			Vector3 dist = cutPoint - cutObject->GetPosition();

			//�ؒf�\������蒷��������
			if (dist.LengthSq() > cutDistance * cutDistance)
			{
				//�ؒf�������̃��[�v��
				continue;
			}

			//�ؒf�\�����ɂ���̂Őؒf
			cutObject->Divide(cutNormal, cutPoint);
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