#include "stdafx.h"
#include "TwoOnFrontTriangle.h"

namespace Util
{
	void TwoOnFrontTriangle::MakeTriangles()
	{
		//1�ڂ̐V���_�ƑΊp�̒��_�łł��������ƁA�c��2�̒��_�����ꂼ��Ȃ����O�p�`2���\���̎O�p�`
		
		//�V���_1�A�V���_1�̑Ίp�̒��_�A�c��̒��_��1��(�V���_2)�ŎO�p�`���쐬
		FrontPushTriangle((*m_newpointArray)[0], m_diagonal, (*m_newpointArray)[1]);

		//�V���_1�A�V���_1�̑Ίp�̒��_�A�c��̒��_��2��(frontIndexes�̂���m_diagonal����Ȃ���)�ŎO�p�`���쐬
		FrontPushTriangle((*m_newpointArray)[0], m_diagonal, FrontLeftOf(m_diagonal));


		//���̒��_�ƐV���_2�ŗ����̎O�p�`

		//�V���_1�A�V���_2�A����1�̒��_�ŎO�p�`���쐬
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
