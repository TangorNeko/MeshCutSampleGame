#include "stdafx.h"
#include "ThreeOnFrontTriangle.h"

namespace Util
{
	void ThreeOnFrontTriangle::MakeTriangles()
	{
		//3�_�����ʂ��\���ɂ���̂�3�_�̎O�p�`���\���̎O�p�`
		/*
		m_frontIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[0]);
		m_frontIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[1]);
		m_frontIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[2]);
		*/
		FrontPushTriangle(m_triangleData->vertexIndexes[0], m_triangleData->vertexIndexes[1], m_triangleData->vertexIndexes[2]);

		//�����̎O�p�`�͂Ȃ�
	}
}