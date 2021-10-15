#pragma once
namespace Util
{
	class CutSurfaceMaker
	{
	public:
		/**
		 * @brief 初期化
		 * @param segmentSet 線分のインデックスの集合
		 * @param vertexContainer モデルの頂点バッファ
		*/
		void Init(std::set<std::pair<uint16_t, uint16_t>>* segmentSet, std::vector<TkmFile::SVertex>* vertexContainer)
		{
			m_segmentSet = segmentSet;
			m_vertexContainer = vertexContainer;
		}

		/**
		 * @brief 線分のインデックスの集合から座標を取得して新しい集合を作成する
		*/
		void ConvertFromSet();

		/**
		 * @brief 新しい集合から頂点間のリンクを作成する
		*/
		void MakeLinkFromSet();
	private:
		std::set<std::pair<uint16_t, uint16_t>>* m_segmentSet = nullptr;	//切断面上にある線分のインデックスの組み合わせの集合
		std::vector<TkmFile::SVertex>* m_vertexContainer = nullptr;			//モデルの頂点バッファ
		std::vector<Vector3> m_vectorContainer;								//切断面の頂点の可変長配列(重複なし)
		std::map<Vector3, int> m_vectorMap;									//Vector3と切断面の頂点の可変長配列内の位置を格納する連想配列
		std::set<std::pair<int, int>> m_convertedSet;						//m_segmentSetをm_vectorContainer上のインデックスに変換して格納したもの
		std::vector<std::vector<uint16_t>> m_pointLinkArray;				//切断面を構成するリンクの配列
	};
}