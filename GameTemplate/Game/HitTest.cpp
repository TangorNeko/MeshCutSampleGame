#include "stdafx.h"
#include "HitTest.h"

namespace Util
{
	void TriangleDivider::Divide(const TriangleData& triangleData)
	{
		if (m_isPlaneDataRegisterd == false)
		{
			MessageBoxA(nullptr, "�ؒf�ʂ̃f�[�^���ݒ肳��Ă��܂���B", "�G���[", MB_OK);
			std::abort();
		}
		
		DataReset();
		m_triangleData = triangleData;

		//Hoge();
	}

	bool TriangleDivider::IsPlaneDivideTriangle()
	{
		//���ʏ�̓_����e���_�ւ̌��������߂�
		Vector3 toVertexDir[3];
		toVertexDir[0] = m_triangleData.vertices[0] - m_planeData.GetPoint();
		toVertexDir[1] = m_triangleData.vertices[1] - m_planeData.GetPoint();
		toVertexDir[2] = m_triangleData.vertices[2] - m_planeData.GetPoint();
		for (Vector3& toVertex : toVertexDir)
		{
			toVertex.Normalize();
		}


		//�O�p�`�̊e�_�����ʂ̕\���ǂ����ɑ��݂��邩�𒲂ׂ�

		//�e�z��̗̈���m��(���_��3�Ȃ̂ōő�ł�3�܂ł�������Ȃ�)
		//�\�ʑ��ɂ������Ƃ��Ɋi�[����z��
		m_vertexIndexesPack.frontVertexIndexes.reserve(3);
		//���ʑ��ɂ������Ƃ��Ɋi�[����z��
		m_vertexIndexesPack.backVertexIndexes.reserve(3);
		//�ʏ�ɂ������Ƃ��Ɋi�[����z��
		m_vertexIndexesPack.onPlaneVertexIndexes.reserve(3);

		//�e�_�ւ̌����ƕ��ʂ̖@���𒲂ׁA�O���[�v��������B
		for (int i = 0; i < 3; i++)
		{
			float angle = m_planeData.GetNormal().Dot(toVertexDir[i]);

			if (angle > 0.0f + FLT_EPSILON)//0���傫�� = ���̒��_�͕\�ʑ��ɂ���
			{
				m_vertexIndexesPack.frontVertexIndexes.push_back(i);
			}
			else if (angle < 0.0f - FLT_EPSILON)//0��菬���� = ���̒��_�͗��ʑ��ɂ���
			{
				m_vertexIndexesPack.backVertexIndexes.push_back(i);
			}
			else//0 = ���̒��_�͕��ʏ�ɂ���
			{
				m_vertexIndexesPack.onPlaneVertexIndexes.push_back(i);
			}
		}

		//�\�ʂƗ��ʂɈ�ȏジ���_������Ε��ʂ͎O�p�`�𕪊����Ă���
		if (m_vertexIndexesPack.frontVertexIndexes.size() >= 1 && m_vertexIndexesPack.backVertexIndexes.size() >= 1)
		{
			return true;
		}

		return false;
	}

	Vector3 TriangleDivider::GetCrossPoint(const Vector3& startPoint, const Vector3& endPoint)
	{
		//���ʏ�̈�_����e�_�ւ̃x�N�g�������߂�
		Vector3 toStart = startPoint - m_planeData.GetPoint();

		Vector3 toEnd = endPoint - m_planeData.GetPoint();

		//�e�_�ƕ��ʂ̖@������ˉe�̒��������߂�
		float projectionA = m_planeData.GetNormal().Dot(toStart);

		//������̓_�͕��ʂ̗����ɂ���̂Ŕ��]�����@���Ƃ̎ˉe�̒��������߂�
		Vector3 reverseNormal = m_planeData.GetNormal() * -1;
		float projectionB = reverseNormal.Dot(toEnd);

		//�����n�_�͊J�n�n�_����I���n�_�ւ̃x�N�g���� 
		//�X�^�[�g�ւ̎ˉe�̒�����2�̎ˉe�̒��� �� (�S�̂̒����̂������ʏ�̈�_����X�^�[�g�ւ̒����̊���)
		//�������������J�n�n�_�ɑ����Ƌ��܂�
		Vector3 crossPoint = endPoint - startPoint;
		crossPoint *= projectionA / (projectionA + projectionB);
		crossPoint += startPoint;

		return crossPoint;
	}

	int TriangleDivider::GetDividedPoint(std::array<Vector3,2>& points)
	{
		//�V�����ł������_��
		int newPointNum = 0;

		//�������_�̊i�[�C���f�b�N�X
		int pointIndex = 0;

		//�������镽�ʏ�ɎO�p�`�̒��_���������ꍇ�������_�ɒǉ�
		for (auto& onPlaneVertexIndex : m_vertexIndexesPack.onPlaneVertexIndexes)
		{
			//�������_�ɒǉ�
			points[pointIndex] = m_triangleData.vertices[onPlaneVertexIndex];
			//�������_�̊i�[�C���f�b�N�X�͑��₷���A���ɂ��钸�_�Ȃ̂ŐV�����ł������_���͑��₳�Ȃ�
			pointIndex++;
		}

		//�����ʂ̕\���̒��_���痠���̒��_�ɐL�т�����ƕ����ʂƂ̌����_�����߂鎖�ŐV�������_�̍��W����ɓ���
		//NOTE:��dfor�������A���_�͍��v3�����Ȃ��A�Б��̒��_����������Ɣ��Α��̒��_�������邱�Ƃ�
		//�������镽�ʏ�ɒ��_���������ꍇ����ɔ���񐔂����邽��
		//for���̒��g���Ă΂��񐔂�1~2��ŌŒ�ł���
		for (auto& frontVertexIndex : m_vertexIndexesPack.frontVertexIndexes)
		{
			for (auto& backVertexIndex : m_vertexIndexesPack.backVertexIndexes)
			{
				//�\���̒��_���痠���̒��_�ւ̐����ƕ��ʂƂ̌����_�����߁A�������_�ɒǉ�
				points[pointIndex] = GetCrossPoint(m_triangleData.vertices[frontVertexIndex], m_triangleData.vertices[backVertexIndex]);

				
				//���ꂪ�ŏ��̕������_�Ȃ�
				if (m_alreadyGetAnyDividePoint == false)
				{
					//���������̂Ɏg���Ă��Ȃ����_�@�܂�l�p�`�𐶐��������V�������_�ƑΊp������钸�_�̃C���f�b�N�X���擾
					GetLeftoverOfThree(frontVertexIndex, backVertexIndex);

					m_alreadyGetAnyDividePoint = true;
				}

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