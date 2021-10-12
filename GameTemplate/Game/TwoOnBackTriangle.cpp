#include "stdafx.h"
#include "TwoOnBackTriangle.h"

namespace Util
{
	void TwoOnBackTriangle::MakeTriangles()
	{
		//表の頂点と新頂点2つで表側の三角形
		/*
		m_frontIndexBuffer->indices.push_back((*m_newpointArray)[0]);//新頂点1
		m_frontIndexBuffer->indices.push_back((*m_newpointArray)[1]);//新頂点2
		m_frontIndexBuffer->indices.push_back(m_vertexIndexesPack->frontVertexIndexes[0]);//表の頂点(1つしかない)
		*/
		FrontPushTriangle((*m_newpointArray)[0], (*m_newpointArray)[1], m_vertexIndexesPack->frontVertexIndexes[0]);


		//1つ目の新頂点と対角の頂点でできた線分と、残り2つの頂点をそれぞれつないだ三角形2つが裏側の三角形
		/*
		m_backIndexBuffer->indices.push_back((*m_newpointArray)[0]);//新頂点1
		m_backIndexBuffer->indices.push_back(m_diagonal);//新頂点1の対角の頂点
		m_backIndexBuffer->indices.push_back((*m_newpointArray)[1]);//残りの頂点の1つめ(新頂点の2つ目)
		*/
		BackPushTriangle((*m_newpointArray)[0], m_diagonal, (*m_newpointArray)[1]);

		/*
		m_backIndexBuffer->indices.push_back((*m_newpointArray)[0]);//新頂点1
		m_backIndexBuffer->indices.push_back(m_diagonal);//新頂点1の対角の頂点
		m_backIndexBuffer->indices.push_back(BackLeftOf(m_diagonal));//残りの頂点の2つめ(backIndexesのうちm_diagonalじゃない方)
		*/
		BackPushTriangle((*m_newpointArray)[0], m_diagonal, BackLeftOf(m_diagonal));
	}

	uint16_t TwoOnBackTriangle::BackLeftOf(uint16_t diagonalIndex)
	{
		if (m_vertexIndexesPack->backVertexIndexes[0] != diagonalIndex)
		{
			return m_vertexIndexesPack->backVertexIndexes[0];
		}
		else
		{
			return m_vertexIndexesPack->backVertexIndexes[1];
		}
	}
}