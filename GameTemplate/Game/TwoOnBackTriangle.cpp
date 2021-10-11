#include "stdafx.h"
#include "TwoOnBackTriangle.h"

namespace Util
{
	void TwoOnBackTriangle::MakeTriangles()
	{
		//�\�̒��_�ƐV���_2�ŕ\���̎O�p�`
		m_frontIndexBuffer->indices.push_back((*m_newpointArray)[0]);//�V���_1
		m_frontIndexBuffer->indices.push_back((*m_newpointArray)[1]);//�V���_2
		m_frontIndexBuffer->indices.push_back(m_vertexIndexesPack->frontVertexIndexes[0]);//�\�̒��_(1�����Ȃ�)

		//1�ڂ̐V���_�ƑΊp�̒��_�łł��������ƁA�c��2�̒��_�����ꂼ��Ȃ����O�p�`2�������̎O�p�`
		m_backIndexBuffer->indices.push_back((*m_newpointArray)[0]);//�V���_1
		m_backIndexBuffer->indices.push_back(m_diagonal);//�V���_1�̑Ίp�̒��_
		m_backIndexBuffer->indices.push_back((*m_newpointArray)[1]);//�c��̒��_��1��(TODO:�ォ��ύX�A�V���_��2�ڂɂȂ邩��?)

		m_backIndexBuffer->indices.push_back((*m_newpointArray)[0]);//�V���_1
		m_backIndexBuffer->indices.push_back(m_diagonal);//�V���_1�̑Ίp�̒��_
		m_backIndexBuffer->indices.push_back(BackLeftOf(m_diagonal));//�c��̒��_��2��(TODO:�ォ��ύX�AbackIndexes�̂���m_diagonal����Ȃ����ɂȂ肻��)
	}

	uint16_t TwoOnBackTriangle::BackLeftOf(uint16_t diagonalIndex)
	{
		for (auto index : m_vertexIndexesPack->backVertexIndexes)
		{
			if (index != diagonalIndex)
			{
				return index;
			}
		}

		MessageBoxA(nullptr, "��v���Ȃ��C���f�b�N�X�������ł��܂���ł����B", "�G���[", MB_OK);
		std::abort();
		return -1;
	}
}