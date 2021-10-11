#include "stdafx.h"
#include "ThreeOnFrontTriangle.h"

namespace Util
{
	void ThreeOnFrontTriangle::MakeTriangles()
	{
		//3点が平面より表側にあるので3点の三角形が表側の三角形
		/*
		m_frontIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[0]);
		m_frontIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[1]);
		m_frontIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[2]);
		*/
		FrontPushTriangle(m_triangleData->vertexIndexes[0], m_triangleData->vertexIndexes[1], m_triangleData->vertexIndexes[2]);

		//裏側の三角形はない
	}
}