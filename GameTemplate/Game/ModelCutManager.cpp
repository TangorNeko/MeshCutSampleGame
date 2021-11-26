#include "stdafx.h"
#include "ModelCutManager.h"

namespace Game
{
	ModelCutManager* ModelCutManager::m_instance = nullptr;

	void ModelCutManager::AddCuttable(SkinModelRender* cuttableModel)
	{
		//切断フラグがtrueなら
		if (cuttableModel->GetDivideFlag() == true)
		{
			//カットするモデルのコンテナに挿入
			m_cutModelPtrSet.insert(cuttableModel);
		}
	}

	void ModelCutManager::QueryCut(const Vector3& cutNormal, const Vector3& cutPoint, float cutDistance)
	{
		//切断モデルのリストを走査
		for (auto* cutObject : m_cutModelPtrSet)
		{
			//切断面との切断モデルの距離を調べる
			Vector3 dist = cutPoint - cutObject->GetPosition();

			//切断可能距離より長かったら
			if (dist.LengthSq() > cutDistance * cutDistance)
			{
				//切断せず次のループへ
				continue;
			}

			//切断可能距離にいるので切断
			cutObject->Divide(cutNormal, cutPoint);
		}

		//切断の結果新しくできた切断可能モデルをリストに積んでいく
		for (auto* addObject : m_nextCutModelPtrSet)
		{
			m_cutModelPtrSet.insert(addObject);
		}

		//新しくできた切断可能モデルのリストを削除
		m_nextCutModelPtrSet.clear();
	}
}