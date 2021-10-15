#include "stdafx.h"
#include "OneOnPlaneTriangle.h"

namespace Util
{
	void OneOnPlaneTriangle::MakeTriangles()
	{
		//1つ目の新頂点には平面と重なった元の点が入っている。
		//2つ目の新頂点と対角の頂点でできた線分と、表側の1頂点をつないだ三角形が表側の三角形

		//新頂点2、新頂点2の対角の頂点、表の1つの頂点で三角形を作成
		FrontPushTriangle((*m_newpointArray)[1], m_diagonal, m_vertexIndexesPack->frontVertexIndexes[0]);

		//2つ目の新頂点と対角の頂点でできた線分と、裏側の1頂点をつないだ三角形が裏側の三角形

		//新頂点2、新頂点2の対角の頂点、裏の1つの頂点で三角形を作成
		BackPushTriangle((*m_newpointArray)[1], m_diagonal, m_vertexIndexesPack->backVertexIndexes[0]);
	}
}