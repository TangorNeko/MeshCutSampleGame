#include "stdafx.h"
#include "OneOnPlaneTriangle.h"

namespace Util
{
	void OneOnPlaneTriangle::MakeTriangles()
	{
		//1�ڂ̐V���_�ɂ͕��ʂƏd�Ȃ������̓_�������Ă���B
			//2�ڂ̐V���_�ƑΊp�̒��_�łł��������ƁA�\����1���_���Ȃ����O�p�`���\���̎O�p�`
		m_frontIndexBuffer->indices.push_back((*m_newpointArray)[1]);//�V���_2
		m_frontIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[m_diagonal]);//�V���_2�̑Ίp�̒��_
		m_frontIndexBuffer->indices.push_back(m_vertexIndexesPack->frontVertexIndexes[0]);//�\�̒��_(1�����Ȃ�)

		//2�ڂ̐V���_�ƑΊp�̒��_�łł��������ƁA������1���_���Ȃ����O�p�`�������̎O�p�`
		m_backIndexBuffer->indices.push_back((*m_newpointArray)[1]);//�V���_2
		m_backIndexBuffer->indices.push_back(m_triangleData->vertexIndexes[m_diagonal]);//�V���_2�̑Ίp�̒��_
		m_backIndexBuffer->indices.push_back(m_vertexIndexesPack->backVertexIndexes[0]);//���̒��_(1�����Ȃ�)
	}
}