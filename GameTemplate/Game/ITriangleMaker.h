#pragma once

#include "tkFile/TkmFile.h"
#include "TriangleDivideStruct.h"

namespace Util
{
	using NewPointArray = std::array<uint16_t, 2>;

	class ITriangleMaker
	{
	public:
		ITriangleMaker() {}

		virtual ~ITriangleMaker() {}

		/**
		 * @brief 三角形を作るためのデータをセット
		 * @param[in] newpointArray 新頂点のインデックスの配列
		 * @param[in] diagonal 1つめの新頂点の対角にある頂点のインデックス
		 * @param[in] vertexIndexesPack 分割された頂点のインデックス
		 * @param[in] triangleData 三角形のデータ
		*/
		void SetData(NewPointArray* newpointArray, int diagonal, VertexIndexesPack* vertexIndexesPack, TriangleData* triangleData)
		{
			m_newpointArray = newpointArray;
			m_diagonal = diagonal;
			m_vertexIndexesPack = vertexIndexesPack;
			m_triangleData = triangleData;
		}

		/**
		 * @brief 分割後のデータを受け取る用のインデックスバッファをセット
		 * @param[out] frontIndexBuffer 表側のメッシュのインデックスバッファ
		 * @param[out] backIndexBuffer 裏側のメッシュのインデックスバッファ
		*/
		void SetArray(TkmFile::SIndexbuffer16* frontIndexBuffer, TkmFile::SIndexbuffer16* backIndexBuffer)
		{
			m_frontIndexBuffer = frontIndexBuffer;
			m_backIndexBuffer = backIndexBuffer;
		}

		/**
		 * @brief インデックスバッファを構成する
		*/
		virtual void MakeTriangles() = 0;
	protected:
		NewPointArray* m_newpointArray = nullptr;				//新頂点のインデックスの配列
		int m_diagonal = -1;									//1つめの新頂点の対角にある頂点のインデックス
		VertexIndexesPack* m_vertexIndexesPack = nullptr;		//分割された頂点のインデックス
		TriangleData* m_triangleData = nullptr;					//三角形のデータ

		TkmFile::SIndexbuffer16* m_frontIndexBuffer = nullptr;	//表側のメッシュのインデックスバッファ
		TkmFile::SIndexbuffer16* m_backIndexBuffer = nullptr;	//裏側のメッシュのインデックスバッファ
	};
}