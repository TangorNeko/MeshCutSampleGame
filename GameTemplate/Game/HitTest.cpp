#include "stdafx.h"
#include "HitTest.h"

namespace Util
{
	bool IsPlaneDivideTriangle(const PlaneData& planeData, const TriangleData& triangleData, VerticesPack& verticesPack)
	{
		//���ʂ̖@�����擾
		Vector3 planeNormal = planeData.planeNormal;
		planeNormal.Normalize();


		//���ʏ�̓_����e���_�ւ̌��������߂�
		Vector3 toVertexDir[3];
		toVertexDir[0] = triangleData.vertices[0] - planeData.planePoint;
		toVertexDir[1] = triangleData.vertices[1] - planeData.planePoint;
		toVertexDir[2] = triangleData.vertices[2] - planeData.planePoint;
		for (Vector3& toVertex : toVertexDir)
		{
			toVertex.Normalize();
		}


		//�O�p�`�̊e�_�����ʂ̕\���ǂ����ɑ��݂��邩�𒲂ׂ�

		//�e�z��̗̈���m��(���_��3�Ȃ̂ōő�ł�3�܂ł�������Ȃ�)
		//�\�ʑ��ɂ������Ƃ��Ɋi�[����z��
		verticesPack.frontVertices.reserve(3);
		//���ʑ��ɂ������Ƃ��Ɋi�[����z��
		verticesPack.backVertices.reserve(3);
		//�ʏ�ɂ������Ƃ��Ɋi�[����z��
		verticesPack.onPlaneVertices.reserve(3);

		//�e�_�ւ̌����ƕ��ʂ̖@���𒲂ׁA�O���[�v��������B
		for (int i = 0; i < 3; i++)
		{
			float angle = planeNormal.Dot(toVertexDir[i]);

			if (angle > 0.0f + FLT_EPSILON)//0���傫�� = ���̒��_�͕\�ʑ��ɂ���
			{
				verticesPack.frontVertices.push_back(triangleData.vertices[i]);
			}
			else if (angle < 0.0f - FLT_EPSILON)//0��菬���� = ���̒��_�͗��ʑ��ɂ���
			{
				verticesPack.backVertices.push_back(triangleData.vertices[i]);
			}
			else//0 = ���̒��_�͕��ʏ�ɂ���
			{
				verticesPack.onPlaneVertices.push_back(triangleData.vertices[i]);
			}
		}

		//�\�ʂƗ��ʂɈ�ȏジ���_������Ε��ʂ͎O�p�`�𕪊����Ă���
		if (verticesPack.frontVertices.size() >= 1 && verticesPack.backVertices.size() >= 1)
		{
			return true;
		}

		return false;
	}
}