#include "stdafx.h"
#include "ThreeOnBackTriangle.h"

namespace Util
{
	void ThreeOnBackTriangle::MakeTriangles()
	{
		//�\���̎O�p�`�͂Ȃ�
		
		//3�_�����ʂ�藠���ɂ���̂�3�_�̎O�p�`�������̎O�p�`
		m_backIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[0]);
		m_backIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[1]);
		m_backIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[2]);
	}
}
