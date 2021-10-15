#include "stdafx.h"
#include "TwoOnBackTriangle.h"

namespace Util
{
	void TwoOnBackTriangle::MakeTriangles()
	{
		//表の頂点と新頂点2つで表側の三角形

		//新頂点1、新頂点2、表の1つの頂点で三角形を作成
		FrontPushTriangle((*m_newpointArray)[0], (*m_newpointArray)[1], m_vertexIndexesPack->frontVertexIndexes[0]);


		//1つ目の新頂点と対角の頂点でできた線分と、残り2つの頂点をそれぞれつないだ三角形2つが裏側の三角形
		
		//新頂点1、新頂点1の対角の頂点、残りの頂点の1つめ(新頂点2)で三角形を作成
		BackPushTriangle((*m_newpointArray)[0], m_diagonal, (*m_newpointArray)[1]);

		//新頂点1、新頂点1の対角の頂点、残りの頂点の2つめ(backIndexexのうちm_diagonalじゃない方)で三角形を作成
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