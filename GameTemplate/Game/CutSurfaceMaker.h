#pragma once
#include "tkFile/TkmFile.h"

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

		/**
		 * @brief 切断面の頂点を2D空間に変換する
		 * @param normal 切断面の法線
		 * @return 切断面の生成の必要あり:true 生成の必要なし:false
		*/
		bool CalcIn2D(const Vector3& normal);

		/**
		 * @brief このリンクは時計回りか?
		 * @param pointLink 切断面の1つのリンク
		 * @return 時計回り:true 反時計回り:false
		*/
		bool IsClockwise(const std::vector<uint16_t> pointLink);

		/**
		 * @brief 切断面の生成
		 * @param vertexBuffer モデルの頂点バッファ
		 * @param frontIndexBufferArray 表側のインデックスバッファの配列
		 * @param backIndexBufferArray 裏側のインデックスバッファの配列
		*/
		void MakeSurface(std::vector<TkmFile::SVertex>* vertexBuffer,std::vector<TkmFile::SIndexbuffer16>* frontIndexBufferArray, std::vector<TkmFile::SIndexbuffer16>* backIndexBufferArray);
	private:
		std::set<std::pair<uint16_t, uint16_t>>* m_segmentSet = nullptr;	//切断面上にある線分のインデックスの組み合わせの集合
		std::vector<TkmFile::SVertex>* m_vertexContainer = nullptr;			//モデルの頂点バッファ
		std::vector<Vector3> m_vectorContainer;								//切断面の頂点の可変長配列(重複なし)
		std::map<Vector3, int> m_vectorMap;									//Vector3と切断面の頂点の可変長配列内の位置を格納する連想配列
		std::set<std::pair<int, int>> m_convertedSet;						//m_segmentSetをm_vectorContainer上のインデックスに変換して格納したもの
		std::vector<std::vector<uint16_t>> m_pointLinkArray;				//切断面を構成するリンクの配列
		std::vector<Vector2> m_2DArray;										//切断面上の頂点の2D空間での座標(m_vectorContainerと対応)
		std::map<Vector2, int> m_2DMap;										//切断面上の頂点の2D空間での座標とm_pointLinkArrayの何番目にあるかの連想配列
		Vector3 m_ex = Vector3::Zero;										//切断面座標系の基底軸
		Vector3 m_ey = Vector3::Zero;
		Vector3 m_ez = Vector3::Zero;
	};
}