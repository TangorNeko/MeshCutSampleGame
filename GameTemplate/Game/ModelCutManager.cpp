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

	void ModelCutManager::QueryCut(const Vector3& cutNormal, const Vector3& cutPoint, std::function<bool(const SkinModelRender* modelRender)> cutJudgeFunc)
	{
		//切断モデルのリストを走査
		for (auto* cutObject : m_cutModelPtrSet)
		{
			//切断するかの判定関数を実行
			bool divideJudge = cutJudgeFunc(cutObject);

			//切断すると判定されたら切断
			if (divideJudge)
			{
				cutObject->Divide(cutNormal, cutPoint);
			}
		}

		//切断の結果新しくできた切断可能モデルをリストに積んでいく
		for (auto* addObject : m_nextCutModelPtrSet)
		{
			m_cutModelPtrSet.insert(addObject);
		}

		//新しくできた切断可能モデルのリストを削除
		m_nextCutModelPtrSet.clear();
	}

	void ModelCutManager::QueryCut(const Vector3& cutNormal, std::function<bool(const SkinModelRender* modelRender)> cutJudgeFunc)
	{
		//切断モデルのリストを走査
		for (auto* cutObject : m_cutModelPtrSet)
		{
			//切断するかの判定関数を実行
			bool divideJudge = cutJudgeFunc(cutObject);

			//切断すると判定されたら切断
			if (divideJudge)
			{
				//切断原点から切断
				cutObject->Divide(cutNormal, cutObject->GetPosition());
			}
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