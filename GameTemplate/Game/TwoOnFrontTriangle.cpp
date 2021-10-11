#include "stdafx.h"
#include "TwoOnFrontTriangle.h"

namespace Util
{
	void TwoOnFrontTriangle::MakeTriangles()
	{
		//1つ目の新頂点と対角の頂点でできた線分と、残り2つの頂点をそれぞれつないだ三角形2つが表側の三角形
		m_frontIndexBuffer->indices.push_back((*m_newpointArray)[0]);//新頂点1
		m_frontIndexBuffer->indices.push_back(m_diagonal);//新頂点1の対角の頂点
		m_frontIndexBuffer->indices.push_back((*m_newpointArray)[1]);//残りの頂点の1つめ(新頂点の2つ目)

		m_frontIndexBuffer->indices.push_back((*m_newpointArray)[0]);//1つめの新頂点
		m_frontIndexBuffer->indices.push_back(m_diagonal);//対角の頂点
		m_frontIndexBuffer->indices.push_back(FrontLeftOf(m_diagonal));//残りの頂点の2つめ(frontIndexesのうちm_diagonalじゃない方)

		//裏の頂点と新頂点2つで裏側の三角形
		m_backIndexBuffer->indices.push_back((*m_newpointArray)[0]);//新頂点1
		m_backIndexBuffer->indices.push_back((*m_newpointArray)[1]);//新頂点2
		m_backIndexBuffer->indices.push_back(m_vertexIndexesPack->backVertexIndexes[0]);//裏の頂点(1つしかない)
	}

	uint16_t TwoOnFrontTriangle::FrontLeftOf(uint16_t diagonalIndex)
	{
		for (auto index : m_vertexIndexesPack->frontVertexIndexes)
		{
			if (index != diagonalIndex)
			{
				return index;
			}
		}

		MessageBoxA(nullptr, "一致しないインデックスを検索できませんでした。", "エラー", MB_OK);
		std::abort();
		return -1;
	}
}
