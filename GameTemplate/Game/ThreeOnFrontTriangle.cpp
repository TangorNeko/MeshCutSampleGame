#include "stdafx.h"
#include "ThreeOnFrontTriangle.h"

namespace Util
{
	void ThreeOnFrontTriangle::MakeTriangles()
	{
		//3�_�����ʂ��\���ɂ���̂�3�_�̎O�p�`���\���̎O�p�`

		//3�_�ŎO�p�`���쐬
		FrontPushTriangle(m_triangleData->vertexIndexes[0], m_triangleData->vertexIndexes[1], m_triangleData->vertexIndexes[2]);

		//�����̎O�p�`�͂Ȃ�
	}
}