#include "stdafx.h"
#include "ThreeOnBackTriangle.h"

namespace Util
{
	void ThreeOnBackTriangle::MakeTriangles()
	{
		//�\���̎O�p�`�͂Ȃ�
		
		//3�_�����ʂ�藠���ɂ���̂�3�_�̎O�p�`�������̎O�p�`

		//3�_�ŎO�p�`���쐬
		BackPushTriangle(m_triangleData->vertexIndexes[0], m_triangleData->vertexIndexes[1], m_triangleData->vertexIndexes[2]);
	}
}
