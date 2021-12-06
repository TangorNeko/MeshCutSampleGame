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
		 * @brief インスタンスの取得
		 * @return CubObjectManagerインスタンス 
		*/
		static ModelCutManager* GetInstance()
		{
			return m_instance;
		}

		/**
		 * @brief 切断するモデルのリストに追加する
		 * @param cuttableModel 切断するモデル
		*/
		void AddCuttable(SkinModelRender* cuttableModel);

		/**
		 * @brief 指定したモデルを切断するモデルのリストから
		 * @param cuttableModel 
		*/
		void RemoveCuttable(SkinModelRender* cuttableModel)
		{
			//切断するモデルのリストから削除する
			auto it = m_cutModelPtrSet.find(cuttableModel);
			if (it != m_cutModelPtrSet.end())
			{
				m_cutModelPtrSet.erase(it);
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
			m_nextCutModelPtrSet.insert(cuttableModel);
		}

		//TODO:cutPointと渡す版と渡さない版の引数の渡し方が分かりづらい、構造体にする?

		/**
		 * @brief m_cutModelPtrSetに登録されているモデルを、渡した関数によって判断してcutPointから切断する
		 * @param cutNormal 切断面の一点からの法線の向き
		 * @param cutPoint 切断面の一点の座標
		 * @param cutForce 切断後のモデルに与える力
		 * @param cutJudgeFunc 切断するかを判定する関数
		*/
		void QueryCut(const Vector3& cutNormal, const Vector3& cutPoint,const Vector3& cutForce, std::function<bool(const SkinModelRender* modelRender)> cutJudgeFunc);

		/**
		 * @brief m_cutModelPtrSetに登録されているモデルを、渡した関数によって判断してモデルの原点から切断する
		 * @param cutNormal モデルの原点から見た切断面の法線の向き
		 * @param cutForce 切断後のモデルに与える力
		 * @param cutJudgeFunc 切断するかを判定する関数
		*/
		void QueryCut(const Vector3& cutNormal,const Vector3& cutForce, std::function<bool(const SkinModelRender* modelRender)> cutJudgeFunc);
	private:
		std::set<SkinModelRender*> m_cutModelPtrSet;		//切断可能なモデルのコンテナ
		std::set<SkinModelRender*> m_nextCutModelPtrSet;	//切断が終了した後追加されるモデルのコンテナ
	};
}
