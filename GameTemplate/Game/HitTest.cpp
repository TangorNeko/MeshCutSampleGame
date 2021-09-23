#include "stdafx.h"
#include "HitTest.h"

namespace Util
{
	bool IsPlaneDivideTriangle(const PlaneData& planeData, const TriangleData& triangleData, VerticesPack& verticesPack)
	{
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
			float angle = planeData.GetNormal().Dot(toVertexDir[i]);

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

	Vector3 GetCrossPoint(const PlaneData& planeData, const Vector3& startPoint, const Vector3& endPoint)
	{
		//���ʏ�̈�_����e�_�ւ̃x�N�g�������߂�
		Vector3 toStart = startPoint - planeData.planePoint;

		Vector3 toEnd = endPoint - planeData.planePoint;

		//�e�_�ƕ��ʂ̖@������ˉe�̒��������߂�
		float projectionA = planeData.GetNormal().Dot(toStart);

		//������̓_�͕��ʂ̗����ɂ���̂Ŕ��]�����@���Ƃ̎ˉe�̒��������߂�
		Vector3 reverseNormal = planeData.GetNormal() * -1;
		float projectionB = reverseNormal.Dot(toEnd);

		//�����n�_�͊J�n�n�_����I���n�_�ւ̃x�N�g���� 
		//�X�^�[�g�ւ̎ˉe�̒�����2�̎ˉe�̒��� �� (�S�̂̒����̂������ʏ�̈�_����X�^�[�g�ւ̒����̊���)
		//�������������J�n�n�_�ɑ����Ƌ��܂�
		Vector3 crossPoint = endPoint - startPoint;
		crossPoint *= projectionA / (projectionA + projectionB);
		crossPoint += startPoint;

		return crossPoint;
	}

	int GetDividedPoint(const PlaneData& planeData, const VerticesPack& verticesPack,std::array<Vector3,2>& points)
	{
		//�V�����ł������_��
		int newPointNum = 0;

		//�������_�̊i�[�C���f�b�N�X
		int pointIndex = 0;

		//�������镽�ʏ�ɎO�p�`�̒��_���������ꍇ�������_�ɒǉ�
		for (auto& onPlaneVertices : verticesPack.onPlaneVertices)
		{
			//�������_�ɒǉ�
			points[pointIndex] = onPlaneVertices;
			//�������_�̊i�[�C���f�b�N�X�͑��₷���A���ɂ��钸�_�Ȃ̂ŐV�����ł������_���͑��₳�Ȃ�
			pointIndex++;
		}

		//�����ʂ̕\���̒��_���痠���̒��_�ɐL�т�����ƕ����ʂƂ̌����_�����߂鎖�ŐV�������_�̍��W����ɓ���
		//NOTE:��dfor�������A���_�͍��v3�����Ȃ��A�Б��̒��_����������Ɣ��Α��̒��_�������邱�Ƃ�
		//�������镽�ʏ�ɒ��_���������ꍇ����ɔ���񐔂����邽��
		//for���̒��g���Ă΂��񐔂�1~2��ŌŒ�ł���
		for (auto& frontVertice : verticesPack.frontVertices)
		{
			for (auto& backVertice : verticesPack.backVertices)
			{
				//�\���̒��_���痠���̒��_�ւ̐����ƕ��ʂƂ̌����_�����߁A�������_�ɒǉ�
				points[pointIndex] = GetCrossPoint(planeData, frontVertice, backVertice);

				//�V�������_�Ȃ̂ŐV�����ł������_���𑝂₷
				newPointNum++;

				//�������_�̊i�[�C���f�b�N�X�𑝂₷
				pointIndex++;
			}
		}

		//�V�����ł������_����Ԃ�
		return newPointNum;
	}
}