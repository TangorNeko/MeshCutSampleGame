#pragma once

#include "tkFile/TkmFile.h"
#include "TriangleDivideStruct.h"
namespace Util
{
	using VertexBuffer = std::vector<TkmFile::SVertex>;
	using IndexBuffer = TkmFile::SIndexbuffer16;
	class ITriangleMaker;

	class TriangleDivider
	{
	public:

		/**
		 * @brief 初期化
		 * @param[in] planeData 分割に使用する平面のデータ
		 * @param[in,out] vertexBufferContainer 分割されるメッシュの頂点バッファ
		 * @param[out] frontIndexBuffer 分割後、分割面の表側と判定されたメッシュのインデックスバッファ
		 * @param[out] backIndexBuffer 分割後、分割面の裏側と判定されたメッシュのインデックスバッファ
		 * @param[out] newVertexContainer 新しく生成された頂点のインデックスを格納する連想配列
		*/
		void Init(const PlaneData& planeData,
			std::vector<TkmFile::SVertex>* vertexBufferContainer)
		{
			m_planeData = planeData;
			m_vertexBuffer = vertexBufferContainer;
			m_isDataSet = true;

			m_isInited = m_isDataSet && m_isReceiverSet;
		}

		void InitReceiver(TkmFile::SIndexbuffer16* frontIndexBuffer,
			TkmFile::SIndexbuffer16* backIndexBuffer)
		{
			m_frontIndexBuffer = frontIndexBuffer;
			m_backIndexBuffer = backIndexBuffer;
			m_isReceiverSet = true;

			m_isInited = m_isDataSet && m_isReceiverSet;
		}

		bool DivideCheck(const TriangleData& triangleData);

		/**
		 * @brief 分割の実行(仮)　必ず分割平面をセットしてから呼ぶ事
		 * @param triangleData[in] 分割を実行する三角形のデータ
		*/
		void Divide(const TriangleData& triangleData);

		void SetCutSeg(std::set<std::pair<uint16_t, uint16_t>>* cutSurfaceSegmentSet)
		{
			m_cutSurfaceSegmentSet = cutSurfaceSegmentSet;
		}

	private:

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

			//新頂点対角のインデックスを初期化
			m_diagonalPoint = -1;
		}

		bool IsDivide();

		/**
		 * @brief 三角形がどう分割されているかを判定し、適切なTriangleMakerを返す
		 * @return 分割のされ方に応じたTriangleMaker
		*/
		ITriangleMaker* CheckHowDivided();

		/**
		 * @brief 三角形の頂点をグループ分けする
		*/
		void VertexGrouping();

		/**
		 * @brief 平面と線分との交差地点を求める(ヒットしている前提)
		 * @param[in] startPoint 線分の開始位置
		 * @param[in] endPoint 線分の終了位置
		 * @return 交差地点の頂点のインデックス
		*/
		uint16_t GetCrossPoint(const TkmFile::SVertex& startVertex, const TkmFile::SVertex& endVertex);

		/**
		 * @brief 0,1,2のインデックス番号のうち含まれていない物を返す
		 * @param[in] first 1つ目のインデックスの数字
		 * @param[in] second 2つ目のインデックスの数字
		 * @return 最後に残ったインデックスの数字
		*/
		int GetLeftoverOfThree(int first, int second)
		{
			return m_sumOfIndexes - first - second;
		}

		/**
		 * @brief 平面とグループ分けされた頂点から分割された部分の頂点を求める
		 * @param[in] verticesPack 頂点のデータ(グループ分け済)
		 * @param[out] points 分割部分の頂点の配列(要素数2)
		 * @return 四角形ができる時、一つ目の新頂点の対角線を構成する点のインデックス
		*/
		void GetDividedPoint();

		void InsertSortedPairToSet(uint16_t i0, uint16_t i1)
		{
			if (i0 < i1)
			{
				m_cutSurfaceSegmentSet->insert(std::make_pair(i0, i1));
			}
			else
			{
				m_cutSurfaceSegmentSet->insert(std::make_pair(i1, i0));
			}
		}
	private:
		bool m_isDataSet = false;
		bool m_isReceiverSet = false;
		bool m_isInited = false;					//初期化されている?
		bool m_alreadyGetAnyDividePoint = false;	//すでに分割頂点を1つでも取得した?
		int m_sumOfIndexes;							//3つのインデックスの合計
		PlaneData m_planeData;						//分割平面データ
		TriangleData m_triangleData;				//分割される三角形のデータ
		VertexIndexesPack m_vertexIndexesPack;		//面の裏・表・上に存在する頂点のインデックスデータ
		std::array<uint16_t, 2> m_newpointArray;	//平面と交差した地点の頂点のインデックス配列
		int m_diagonalPoint = -1;					//新頂点の対角にある頂点のインデックス
		VertexBuffer* m_vertexBuffer = nullptr;		//元の頂点バッファ
		IndexBuffer* m_frontIndexBuffer = nullptr;	//表側のメッシュのインデックスバッファ
		IndexBuffer* m_backIndexBuffer = nullptr;	//裏側のメッシュのインデックスバッファ
		std::map<TkmFile::SVertex, uint16_t> m_newVertexMap;//頂点バッファをキーとして、既に同じ頂点が登録されていた場合そのインデックスを返す連想配列
		std::set<std::pair<uint16_t, uint16_t>>* m_cutSurfaceSegmentSet = nullptr;//切断面にできた線分の開始、終了インデックスを格納する集合
	};
}