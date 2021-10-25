#pragma once

namespace Game
{
	class ModelCutManager
	{
	private:
		//NOTE:シングルトンパターン。　シングルトンパターンの是非は後ほど検討する
		//ゲームシーン以外で切断を行わないならGameSceneとかに紐付けたほうがいい?
		ModelCutManager() = default;
		~ModelCutManager() = default;
		static ModelCutManager* m_instance;

	public:
		/**
		 * @brief インスタンスの生成
		*/
		static void CreateInstance()
		{
			if (!m_instance)
			{
				m_instance = new ModelCutManager;
			}
		}

		/**
		 * @brief インスタンスの削除
		*/
		static void DeleteInstance()
		{
			delete m_instance;
			m_instance = nullptr;
		}

		/**
		 * @brief 
		 * @return CubObjectManagerインスタンス 
		*/
		static ModelCutManager* GetInstance()
		{
			return m_instance;
		}

		/**
		 * @brief 
		 * @param cuttableModel 切断するモデル
		*/
		void AddCuttable(SkinModelRender* cuttableModel)
		{
			//切断フラグがtrueなら
			if (cuttableModel->GetDivideFlag() == true)
			{
				//カットするモデルのコンテナに挿入
				m_cutModelPtrSet.insert(cuttableModel);
			}
		}

		/**
		 * @brief 現在の切断が終了した後追加されるモデル(切断後の新規モデル)をセット
		 * @param cuttableModel 追加されるモデル
		*/
		void AddNextCuttable(SkinModelRender* cuttableModel)
		{
			//NOTE:そのままm_cutModelPtrSetに挿入すると、切断の結果できたモデルをもう一度切断してしまい
			//その切断でできたモデルをもう一度切断し...のループが起こるので別にセットして後から追加する
			m_nextCutModelPtrList.insert(cuttableModel);
		}

		/**
		 * @brief m_cutModelPtrSetに登録されているモデルのうち、cutPointから一定距離以内のものを切断する
		 * @param cutNormal 切断面の一点からの法線の向き
		 * @param cutPoint 切断面の一点の座標
		*/
		void QueryCut(const Vector3& cutNormal, const Vector3& cutPoint, float cutDistance);

	private:
		std::set<SkinModelRender*> m_cutModelPtrSet;		//切断可能なモデルのコンテナ
		std::set<SkinModelRender*> m_nextCutModelPtrList;	//切断が終了した後追加されるモデルのコンテナ
	};
}
