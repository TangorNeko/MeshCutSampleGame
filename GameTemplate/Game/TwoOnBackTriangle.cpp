#include "stdafx.h"
#include "TwoOnBackTriangle.h"

namespace Util
{
	void TwoOnBackTriangle::MakeTriangles()
	{
		//�\�̒��_�ƐV���_2�ŕ\���̎O�p�`
		/*
		m_frontIndexBuffer->indices.push_back((*m_newpointArray)[0]);//�V���_1
		m_frontIndexBuffer->indices.push_back((*m_newpointArray)[1]);//�V���_2
		m_frontIndexBuffer->indices.push_back(m_vertexIndexesPack->frontVertexIndexes[0]);//�\�̒��_(1�����Ȃ�)
		*/
		FrontPushTriangle((*m_newpointArray)[0], (*m_newpointArray)[1], m_vertexIndexesPack->frontVertexIndexes[0]);


		//1�ڂ̐V���_�ƑΊp�̒��_�łł��������ƁA�c��2�̒��_�����ꂼ��Ȃ����O�p�`2�������̎O�p�`
		/*
		m_backIndexBuffer->indices.push_back((*m_newpointArray)[0]);//�V���_1
		m_backIndexBuffer->indices.push_back(m_diagonal);//�V���_1�̑Ίp�̒��_
		m_backIndexBuffer->indices.push_back((*m_newpointArray)[1]);//�c��̒��_��1��(�V���_��2��)
		*/
		BackPushTriangle((*m_newpointArray)[0], m_diagonal, (*m_newpointArray)[1]);

		/*
		m_backIndexBuffer->indices.push_back((*m_newpointArray)[0]);//�V���_1
		m_backIndexBuffer->indices.push_back(m_diagonal);//�V���_1�̑Ίp�̒��_
		m_backIndexBuffer->indices.push_back(BackLeftOf(m_diagonal));//�c��̒��_��2��(backIndexes�̂���m_diagonal����Ȃ���)
		*/
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