#include "stdafx.h"
#include "OneOnPlaneTriangle.h"

namespace Util
{
	void OneOnPlaneTriangle::MakeTriangles()
	{
		//1つ目の新頂点には平面と重なった元の点が入っている。
			//2つ目の新頂点と対角の頂点でできた線分と、表側の1頂点をつないだ三角形が表側の三角形
		m_frontIndexBuffer->indices.push_back((*m_newpointArray)[1]);//新頂点2
		m_frontIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[m_diagonal]);//新頂点2の対角の頂点
		m_frontIndexBuffer->indices.push_back(m_vertexIndexesPack->frontVertexIndexes[0]);//表の頂点(1つしかない)

		//2つ目の新頂点と対角の頂点でできた線分と、裏側の1頂点をつないだ三角形が裏側の三角形
		m_backIndexBuffer->indices.push_back((*m_newpointArray)[1]);//新頂点2
		m_backIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[m_diagonal]);//新頂点2の対角の頂点
		m_backIndexBuffer->indices.push_back(m_vertexIndexesPack->backVertexIndexes[0]);//裏の頂点(1つしかない)
	}
}