#include "stdafx.h"
#include "ThreeOnPlaneTriangle.h"

namespace Util
{
	void ThreeOnPlaneTriangle::MakeTriangles()
	{
		//NOTE:���ʏ��3�_�����鎞�͂ǂ���̖ʂɂ��܂�ł��鏈���Ƃ��邱�Ƃɂ��Ă���B
		
		//3�_�̎O�p�`�ŕ\���̎O�p�`
		/*
		m_frontIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[0]);
		m_frontIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[1]);
		m_frontIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[2]);
		*/
		FrontPushTriangle(m_triangleData->vertexIndexes[0], m_triangleData->vertexIndexes[1], m_triangleData->vertexIndexes[2]);

		//3�_�̎O�p�`�ŗ����̎O�p�`
		/*
		m_backIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[0]);
		m_backIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[1]);
		m_backIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[2]);
		*/
		BackPushTriangle(m_triangleData->vertexIndexes[0], m_triangleData->vertexIndexes[1], m_triangleData->vertexIndexes[2]);
	}
}