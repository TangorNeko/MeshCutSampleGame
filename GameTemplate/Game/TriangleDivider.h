#pragma once

#include "tkFile/TkmFile.h"
#include "TriangleDivideStruct.h"

namespace Util
{
	enum DivideState//分割のされ方
	{
		Divided_2OnFront,//分割_表側に頂点2つ
		Divided_2OnBack,//分割_裏側に頂点2つ
		Divided_1OnPlane,//分割_面上に頂点1つ
		NotDivided_3OnFront,//非分割_表側に頂点3つ
		NotDivided_3OnBack,//非分割_裏側に頂点3つ
		Special_3OnPlane,//特殊_面上に頂点3つ
	};

	class TriangleDivider
	{
	public:

		/**
		 * @brief 初期化
		 * @param planeData 分割平面のデータ
		 * @param newVertexContainer MeshDividerの新頂点を
		*/
		void Init(const PlaneData& planeData,
			std::vector<TkmFile::SVertex>* vertexBufferContainer,
			TkmFile::SIndexBuffer32* frontIndexBuffer,
			TkmFile::SIndexBuffer32* backIndexBuffer,
			std::map<std::pair<Vector3, Vector3>, uint32_t>* newVertexContainer)
		{
			m_planeData = planeData;
			m_vertexBufferContainer = vertexBufferContainer;
			m_frontIndexBuffer = frontIndexBuffer;
			m_backIndexBuffer = backIndexBuffer;
			m_newVertexContainer = newVertexContainer;
			m_isInited = true;
		}

		/**
		 * @brief 分割の実行(仮)　必ず分割平面をセットしてから呼ぶ事
		 * @param triangleData[in] 分割を実行する三角形のデータ
		*/
		void Divide(const TriangleData& TriangleData);

		//private:

		/**
		 * @brief データのリセット
		*/
		void DataReset()
		{
			//分割頂点を取得したかのフラグをリセット
			m_alreadyGetAnyDividePoint = false;

			//面の裏・表・上に存在する頂点のインデックスデータをリセット
			m_vertexIndexesPack.frontVertexIndexes.clear();
			m_vertexIndexesPack.backVertexIndexes.clear();
			m_vertexIndexesPack.onPlaneVertexIndexes.clear();
		}

		/**
		 * @brief 分割のされ方を計算
		 * @return 分割のされ方
		*/
		DivideState CalcDivideState();

		/**
		 * @brief 平面が三角形を分割している?
		 * @return 分割のされ方
		*/
		DivideState IsPlaneDivideTriangle();

		/**
		 * @brief 平面と線分との交差地点を求める(ヒットしている前提)
		 * @param startPoint[in] 線分の開始位置
		 * @param endPoint[in] 線分の終了位置
		 * @return 交差地点の頂点のインデックス
		*/
		uint32_t GetCrossPoint(const TkmFile::SVertex& startVertex, const TkmFile::SVertex& endVertex);

		/**
		 * @brief 0,1,2のインデックス番号のうち含まれていない物を返す
		 * @param first 1つ目のインデックスの数字
		 * @param second 2つ目のインデックスの数字
		 * @return 最後に残ったインデックスの数字
		*/
		int GetLeftoverOfThree(int first, int second)
		{
			return 3 - first - second;
		}

		/**
		 * @brief 平面とグループ分けされた頂点から分割された部分の頂点を求める
		 * @param verticesPack[in] 頂点のデータ(グループ分け済)
		 * @param points[out] 分割部分の頂点の配列(要素数2)
		 * @return 四角形ができる時、一つ目の新頂点の対角線を構成する点のインデックス
		*/
		int GetDividedPoint(std::array<uint32_t, 2>& points);

	private:
		bool m_isInited = false;					//初期化されている?
		bool m_alreadyGetAnyDividePoint = false;	//すでに分割頂点を1つでも取得した?
		PlaneData m_planeData;						//分割平面データ
		TriangleData m_triangleData;				//分割される三角形のデータ
		VertexIndexesPack m_vertexIndexesPack;		//面の裏・表・上に存在する頂点のインデックスデータ
		std::vector<TkmFile::SVertex>* m_vertexBufferContainer;	//元の頂点バッファ
		std::map<std::pair<Vector3, Vector3>, uint32_t>* m_newVertexContainer; //分割によってできた新頂点を格納する連想配列のポインタ
		TkmFile::SIndexBuffer32* m_frontIndexBuffer;
		TkmFile::SIndexBuffer32* m_backIndexBuffer;
	};
}