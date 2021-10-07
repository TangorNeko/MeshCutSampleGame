#include "stdafx.h"
#include "TwoOnBackTriangle.h"

namespace Util
{
	void TwoOnBackTriangle::MakeTriangles()
	{
		//表の頂点と新頂点2つで表側の三角形
		m_frontIndexBuffer->indices.push_back((*m_newpointArray)[0]);//新頂点1
		m_frontIndexBuffer->indices.push_back((*m_newpointArray)[1]);//新頂点2
		m_frontIndexBuffer->indices.push_back(m_vertexIndexesPack->frontVertexIndexes[0]);//表の頂点(1つしかない)

		//1つ目の新頂点と対角の頂点でできた線分と、残り2つの頂点をそれぞれつないだ三角形2つが裏側の三角形
		m_backIndexBuffer->indices.push_back((*m_newpointArray)[0]);//新頂点1
		m_backIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[m_diagonal]);//新頂点1の対角の頂点
		m_backIndexBuffer->indices.push_back(1);//残りの頂点の1つめ(TODO:後から変更、新頂点の2つ目になるかな?)

		m_backIndexBuffer->indices.push_back((*m_newpointArray)[0]);//新頂点1
		m_backIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[m_diagonal]);//新頂点1の対角の頂点
		m_backIndexBuffer->indices.push_back(2);//残りの頂点の2つめ(TODO:後から変更、backIndexesのうちm_diagonalじゃない方になりそう)
	}
}