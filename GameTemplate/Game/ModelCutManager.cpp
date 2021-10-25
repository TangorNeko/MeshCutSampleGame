#include "stdafx.h"
#include "ModelCutManager.h"

namespace Game
{
	ModelCutManager* ModelCutManager::m_instance = nullptr;

	void ModelCutManager::QueryCut(const Vector3& cutNormal, const Vector3& cutPoint, float cutDistance)
	{
		for (auto* cutObject : m_cutModelPtrSet)
		{
			Vector3 dist = cutPoint - cutObject->GetPosition();
			if (dist.LengthSq() > cutDistance * cutDistance)
			{
				continue;
			}

			cutObject->Divide(cutNormal, cutPoint);
		}

		for (auto* addObject : m_nextCutModelPtrList)
		{
			m_cutModelPtrSet.insert(addObject);
		}

		m_nextCutModelPtrList.clear();
	}
}