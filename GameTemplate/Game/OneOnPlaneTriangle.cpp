#include "stdafx.h"
#include "OneOnPlaneTriangle.h"

namespace Util
{
	void OneOnPlaneTriangle::MakeTriangles()
	{
		//1�ڂ̐V���_�ɂ͕��ʂƏd�Ȃ������̓_�������Ă���B
		//2�ڂ̐V���_�ƑΊp�̒��_�łł��������ƁA�\����1���_���Ȃ����O�p�`���\���̎O�p�`

		//�V���_2�A�V���_2�̑Ίp�̒��_�A�\��1�̒��_�ŎO�p�`���쐬
		FrontPushTriangle((*m_newpointArray)[1], m_diagonal, m_vertexIndexesPack->frontVertexIndexes[0]);

		//2�ڂ̐V���_�ƑΊp�̒��_�łł��������ƁA������1���_���Ȃ����O�p�`�������̎O�p�`

		//�V���_2�A�V���_2�̑Ίp�̒��_�A����1�̒��_�ŎO�p�`���쐬
		BackPushTriangle((*m_newpointArray)[1], m_diagonal, m_vertexIndexesPack->backVertexIndexes[0]);
	}
}