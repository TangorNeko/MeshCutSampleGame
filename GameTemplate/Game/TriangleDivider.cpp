#include "stdafx.h"
#include "TriangleDivider.h"

namespace Util
{
	void TriangleDivider::Divide(const TriangleData& triangleData)
	{
		if (m_isInited == false)
		{
			MessageBoxA(nullptr, "�ؒf�O�̃f�[�^�̏�����������Ă��܂���B", "�G���[", MB_OK);
			std::abort();
		}
		
		DataReset();
		m_triangleData = triangleData;

		std::array<uint32_t, 2> newpointArray;
		uint32_t diagonal;

		//��������̌��ʂɂ���ĕ���?
		//TODO:�V�I�u�W�F�N�g�Ƃ��ĕ��򂳂����ق����R�[�h�ʓI�ɂ悳����
		switch (IsPlaneDivideTriangle())
		{
		case Divided_2OnFront:
			diagonal = GetDividedPoint(newpointArray);
			//1�ڂ̐V���_�ƑΊp�̒��_�łł��������ƁA�c��2�̒��_�����ꂼ��Ȃ����O�p�`2���\���̎O�p�`
			m_frontIndexBuffer->indices.push_back(newpointArray[0]);//�V���_1
			m_frontIndexBuffer->indices.push_back(diagonal);//�V���_1�̑Ίp�̒��_
			m_frontIndexBuffer->indices.push_back(1);//�c��̒��_��1��(TODO:�ォ��ύX)

			m_frontIndexBuffer->indices.push_back(newpointArray[0]);//1�߂̐V���_
			m_frontIndexBuffer->indices.push_back(diagonal);//�Ίp�̒��_
			m_frontIndexBuffer->indices.push_back(2);//�c��̒��_��2��(TODO:�ォ��ύX)

			//���̒��_�ƐV���_2�ŗ����̎O�p�`
			m_backIndexBuffer->indices.push_back(newpointArray[0]);//�V���_1
			m_backIndexBuffer->indices.push_back(newpointArray[1]);//�V���_2
			m_backIndexBuffer->indices.push_back(m_vertexIndexesPack.backVertexIndexes[0]);//���̒��_(1�����Ȃ�)
			break;
		case Divided_2OnBack:
			diagonal = GetDividedPoint(newpointArray);
			//�\�̒��_�ƐV���_2�ŕ\���̎O�p�`
			m_frontIndexBuffer->indices.push_back(newpointArray[0]);//�V���_1
			m_frontIndexBuffer->indices.push_back(newpointArray[1]);//�V���_2
			m_frontIndexBuffer->indices.push_back(m_vertexIndexesPack.frontVertexIndexes[0]);//�\�̒��_(1�����Ȃ�)

			//1�ڂ̐V���_�ƑΊp�̒��_�łł��������ƁA�c��2�̒��_�����ꂼ��Ȃ����O�p�`2�������̎O�p�`
			m_backIndexBuffer->indices.push_back(newpointArray[0]);//�V���_1
			m_backIndexBuffer->indices.push_back(diagonal);//�V���_1�̑Ίp�̒��_
			m_backIndexBuffer->indices.push_back(1);//�c��̒��_��1��(TODO:�ォ��ύX)

			m_backIndexBuffer->indices.push_back(newpointArray[0]);//�V���_1
			m_backIndexBuffer->indices.push_back(diagonal);//�V���_1�̑Ίp�̒��_
			m_backIndexBuffer->indices.push_back(2);//�c��̒��_��2��(TODO:�ォ��ύX)
			break;
		case Divided_1OnPlane:
			diagonal = GetDividedPoint(newpointArray);
			//1�ڂ̐V���_�ɂ͕��ʂƏd�Ȃ������̓_�������Ă���B
			//2�ڂ̐V���_�ƑΊp�̒��_�łł��������ƁA�\����1���_���Ȃ����O�p�`���\���̎O�p�`
			m_frontIndexBuffer->indices.push_back(newpointArray[1]);//�V���_2
			m_frontIndexBuffer->indices.push_back(diagonal);//�V���_2�̑Ίp�̒��_
			m_frontIndexBuffer->indices.push_back(m_vertexIndexesPack.frontVertexIndexes[0]);//�\�̒��_(1�����Ȃ�)

			//2�ڂ̐V���_�ƑΊp�̒��_�łł��������ƁA������1���_���Ȃ����O�p�`�������̎O�p�`
			m_backIndexBuffer->indices.push_back(newpointArray[1]);//�V���_2
			m_backIndexBuffer->indices.push_back(diagonal);//�V���_2�̑Ίp�̒��_
			m_backIndexBuffer->indices.push_back(m_vertexIndexesPack.backVertexIndexes[0]);//���̒��_(1�����Ȃ�)
			break;
		case NotDivided_3OnFront:
			//3�_�����ʂ��\���ɂ���̂�3�_�̎O�p�`���\���̎O�p�`
			m_frontIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[0]);
			m_frontIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[1]);
			m_frontIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[2]);
			//�����̎O�p�`�͂Ȃ�
			break;
		case NotDivided_3OnBack:
			//�\���̎O�p�`�͂Ȃ�
			//3�_�����ʂ�藠���ɂ���̂�3�_�̎O�p�`�������̎O�p�`
			m_backIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[0]);
			m_backIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[1]);
			m_backIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[2]);
			break;
		case Special_3OnPlane:
			//NOTE:���ʏ��3�_�����鎞�͂ǂ���̖ʂɂ��܂�ł��鏈���Ƃ��邱�Ƃɂ��Ă���B
			//3�_�̎O�p�`�ŕ\���̎O�p�`
			m_frontIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[0]);
			m_frontIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[1]);
			m_frontIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[2]);

			//3�_�̎O�p�`�ŗ����̎O�p�`
			m_backIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[0]);
			m_backIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[1]);
			m_backIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[2]);
			break;
		}
	}

	DivideState TriangleDivider::CalcDivideState()
	{
		int frontSize = m_vertexIndexesPack.frontVertexIndexes.size();
		int backSize = m_vertexIndexesPack.backVertexIndexes.size();
		int onPlaneSize = m_vertexIndexesPack.onPlaneVertexIndexes.size();

		if (frontSize == 3)
		{
			return NotDivided_3OnFront;
		}

		if (backSize == 3)
		{
			return NotDivided_3OnBack;
		}

		if (onPlaneSize == 3)
		{
			return Special_3OnPlane;
		}

		if (frontSize == 2 && backSize == 1)
		{
			return Divided_2OnFront;
		}

		if (frontSize == 1 && backSize == 2)
		{
			return Divided_2OnBack;
		}

		if (frontSize == 1 && backSize == 1 && onPlaneSize == 1)
		{
			return Divided_1OnPlane;
		}

		if (frontSize > backSize)
		{
			return NotDivided_3OnFront;
		}
		else
		{
			return NotDivided_3OnBack;
		}
	}

	DivideState TriangleDivider::IsPlaneDivideTriangle()
	{
		//���ʏ�̓_����e���_�ւ̌��������߂�
		Vector3 toVertexDir[3];
		toVertexDir[0] = m_vertexBufferContainer->at(m_triangleData.vertexIndexes[0]).pos - m_planeData.GetPoint();
		toVertexDir[1] = m_vertexBufferContainer->at(m_triangleData.vertexIndexes[1]).pos - m_planeData.GetPoint();
		toVertexDir[2] = m_vertexBufferContainer->at(m_triangleData.vertexIndexes[2]).pos - m_planeData.GetPoint();
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

		//���_�̕���������擾
		return CalcDivideState();
	}

	uint32_t TriangleDivider::GetCrossPoint(const TkmFile::SVertex& startVertex, const TkmFile::SVertex& endVertex)
	{
		//�J�n���_�ƏI�����_�̍��W���擾
		Vector3 startPoint = startVertex.pos;
		Vector3 endPoint = endVertex.pos;

		//startPoint��endPoint���t�̏ꍇ���ʂ͓����Ȃ̂�map��ł͕ʂɂȂ�̂ŁA
		//�\�[�g���ꂽ�y�A���쐬����
		std::pair<Vector3,Vector3> sortedPair = Vector3::MakeSortedPair(startPoint,endPoint);

		//���łɘA�z�z��Ɍ��ʂ��i�[����Ă���ꍇreturn
		auto knownVertexIndex = m_newVertexContainer->find(sortedPair);
		if (knownVertexIndex != m_newVertexContainer->end())
		{
			return knownVertexIndex->second;
		}
		

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

		//���`��ԗ����v�Z
		Vector3 startToEnd = endPoint - startPoint;
		Vector3 startToCrossPoint = crossPoint - startPoint;
		float lerpRate = startToCrossPoint.Length() / startToEnd.Length();
		
		TkmFile::SVertex newVertex = TkmFile::lerpVertex(lerpRate, startVertex, endVertex);
		//pos�����`�⊮���Ă���̂��������ɂȂ邩������Ȃ��̂Ōv�Z����crossPoint���g�p
		newVertex.pos = crossPoint;

		//���݂̃T�C�Y(���ɒǉ�����v�f�̃C���f�b�N�X�ԍ�)���擾
		uint32_t newVertexIndex = m_vertexBufferContainer->size();
		m_vertexBufferContainer->push_back(newVertex);

		//�V�����ł������_�̃C���f�b�N�X���i�[����A�z�z��ɑ}��
		m_newVertexContainer->insert(std::make_pair(sortedPair, newVertexIndex));

		return newVertexIndex;
	}

	int TriangleDivider::GetDividedPoint(std::array<uint32_t,2>& points)
	{
		//�V�����ł������_��
		int diagonalPointIndex = 0;

		//�������_�̊i�[�C���f�b�N�X
		int pointIndex = 0;

		//�������镽�ʏ�ɎO�p�`�̒��_���������ꍇ�������_�ɒǉ�
		for (auto& onPlaneVertexIndex : m_vertexIndexesPack.onPlaneVertexIndexes)
		{
			//�������_�ɒǉ�
			points[pointIndex] = m_triangleData.vertexIndexes[onPlaneVertexIndex];

			//�������_�̊i�[�C���f�b�N�X�𑝂₷�B
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
				uint32_t newVertexIndex = GetCrossPoint(
					m_vertexBufferContainer->at(m_triangleData.vertexIndexes[frontVertexIndex]),
					m_vertexBufferContainer->at(m_triangleData.vertexIndexes[backVertexIndex]));

				points[pointIndex] = newVertexIndex;

				
				//���ꂪ�ŏ��̕������_�Ȃ�
				if (m_alreadyGetAnyDividePoint == false)
				{
					//���������̂Ɏg���Ă��Ȃ����_�@�܂�l�p�`�𐶐��������V�������_�ƑΊp������钸�_�̃C���f�b�N�X���擾
					diagonalPointIndex = GetLeftoverOfThree(frontVertexIndex, backVertexIndex);

					m_alreadyGetAnyDividePoint = true;
				}

				//�������_�̊i�[�C���f�b�N�X�𑝂₷
				pointIndex++;
			}
		}

		//�V�����ł������_�̑Ίp�̃C���f�b�N�X��Ԃ�
		return diagonalPointIndex;
	}
}