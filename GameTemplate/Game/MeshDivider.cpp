#include "stdafx.h"
#include "MeshDivider.h"
#include "TriangleDivider.h"

namespace Util
{
	void MeshDivider::Divide()
	{
		TriangleDivider triangleDivider;

		PlaneData plane;
		plane.SetNormal(Vector3::Up);
		plane.SetPoint(Vector3::Zero);
		triangleDivider.Init(plane,&m_newVertexContainer);

		//TODO:�C���f�b�N�X�o�b�t�@�ƒ��_�o�b�t�@���󂯎��
		// �C���f�b�N�X�o�b�t�@�̈�̎O�p�`���Ƃ�TriangleDivider�����s?
		//�C���[�W
		//for (auto index : IndexBuffer)
		//{
		//	TriangleData triangleData;
		//	triangleData.vertices[0] = VertexBuffer[index[0]];
		//	triangleData.vertices[1] = VertexBuffer[index[1]];
		//	triangleData.vertices[2] = VertexBuffer[index[2]];
		//
		//	triangleDivider.Divide(triangleData);
		//}

	}
}
