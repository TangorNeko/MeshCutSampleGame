#include "stdafx.h"
#include "TwoOnFrontTriangle.h"

namespace Util
{
	void TwoOnFrontTriangle::MakeTriangles()
	{
		//1�ڂ̐V���_�ƑΊp�̒��_�łł��������ƁA�c��2�̒��_�����ꂼ��Ȃ����O�p�`2���\���̎O�p�`
		/*
		m_frontIndexBuffer->indices.push_back((*m_newpointArray)[0]);//�V���_1
		m_frontIndexBuffer->indices.push_back(m_diagonal);//�V���_1�̑Ίp�̒��_
		m_frontIndexBuffer->indices.push_back((*m_newpointArray)[1]);//�c��̒��_��1��(�V���_��2��)
		*/
		FrontPushTriangle((*m_newpointArray)[0], m_diagonal, (*m_newpointArray)[1]);

		/*
		m_frontIndexBuffer->indices.push_back((*m_newpointArray)[0]);//1�߂̐V���_
		m_frontIndexBuffer->indices.push_back(m_diagonal);//�Ίp�̒��_
		m_frontIndexBuffer->indices.push_back(FrontLeftOf(m_diagonal));//�c��̒��_��2��(frontIndexes�̂���m_diagonal����Ȃ���)
		*/
		FrontPushTriangle((*m_newpointArray)[0], m_diagonal, FrontLeftOf(m_diagonal));


		//���̒��_�ƐV���_2�ŗ����̎O�p�`
		/*
		m_backIndexBuffer->indices.push_back((*m_newpointArray)[0]);//�V���_1
		m_backIndexBuffer->indices.push_back((*m_newpointArray)[1]);//�V���_2
		m_backIndexBuffer->indices.push_back(m_vertexIndexesPack->backVertexIndexes[0]);//���̒��_(1�����Ȃ�)
		*/
		BackPushTriangle((*m_newpointArray)[0], (*m_newpointArray)[1], m_vertexIndexesPack->backVertexIndexes[0]);
	}

	uint16_t TwoOnFrontTriangle::FrontLeftOf(uint16_t diagonalIndex)
	{
		if (m_vertexIndexesPack->frontVertexIndexes[0] != diagonalIndex)
		{
			return m_vertexIndexesPack->frontVertexIndexes[0];
		}
		else
		{
			return m_vertexIndexesPack->frontVertexIndexes[1];
		}
	}
}
