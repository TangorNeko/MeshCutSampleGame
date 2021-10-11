#include "stdafx.h"
#include "ThreeOnPlaneTriangle.h"

namespace Util
{
	void ThreeOnPlaneTriangle::MakeTriangles()
	{
		//NOTE:平面上に3点がある時はどちらの面にも含んでいる処理とすることにしている。
		
		//3点の三角形で表側の三角形
		/*
		m_frontIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[0]);
		m_frontIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[1]);
		m_frontIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[2]);
		*/
		FrontPushTriangle(m_triangleData->vertexIndexes[0], m_triangleData->vertexIndexes[1], m_triangleData->vertexIndexes[2]);

		//3点の三角形で裏側の三角形
		/*
		m_backIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[0]);
		m_backIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[1]);
		m_backIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[2]);
		*/
		BackPushTriangle(m_triangleData->vertexIndexes[0], m_triangleData->vertexIndexes[1], m_triangleData->vertexIndexes[2]);
	}
}