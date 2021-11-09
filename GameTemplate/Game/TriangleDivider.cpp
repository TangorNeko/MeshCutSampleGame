#include "stdafx.h"
#include "TriangleDivider.h"
#include "TriangleMakerHeader.h"

namespace Util
{
	bool TriangleDivider::DivideCheck(const TriangleData& triangleData)
	{
		if (m_isDataSet == false)
		{
			MessageBoxA(nullptr, "�ؒf����O�̃f�[�^�̏�����������Ă��܂���B", "�G���[", MB_OK);
			std::abort();
		}
		//�O�p�`�̃f�[�^�����Z�b�g
		DataReset();

		//�O�p�`�̃f�[�^���Z�b�g
		m_triangleData = triangleData;

		//�O�p�`�̒��_�����ɕ\�A���A�ʏ�ɃO���[�v��������B
		VertexGrouping();

		return IsDivide();
	}

	bool TriangleDivider::IsDivide()
	{
		//�\���A�����A���ʏ�ɂ������_�����邩���擾
		int frontSize = static_cast<int>(m_vertexIndexesPack.frontVertexIndexes.size());
		int backSize = static_cast<int>(m_vertexIndexesPack.backVertexIndexes.size());
		int onPlaneSize = static_cast<int>(m_vertexIndexesPack.onPlaneVertexIndexes.size());

		//�Б���3�_������Ă��邩�A�ʏ��3�_������ꍇ��������Ă��Ȃ�
		//NOTE:�ʏ��3�_������ꍇ�A�ؒf�̍ۂ͕�������Ă���Ɣ��肷�邪�A
		//�ؒf�`�F�b�N�̏ꍇ�͔���1�������ɂȂ�\��������̂ŕ������Ă��Ȃ��Ɣ��肵�Ă���B
		if (frontSize == 3 || backSize == 3 || onPlaneSize == 3) 
		{
			return false;
		}

		//�ʂ̑O��ɂ�1�_���������ꍇ��������Ă���
		if (frontSize >= 1 && backSize >= 1)
		{
			return true;
		}

		//�c��͂ǂ��炩��2�Ɩʏ��1�̃p�^�[���Ȃ̂ŕ�������Ă��Ȃ�
		return false;
	}

	void TriangleDivider::Divide(const TriangleData& triangleData)
	{
		//�f�[�^�̏������O�ɐؒf�����s���ꂽ�ꍇ�G���[�B
		if (m_isInited == false)
		{
			MessageBoxA(nullptr, "�ؒf�O�̃f�[�^�̏�����������Ă��܂���B", "�G���[", MB_OK);
			std::abort();
		}
		
		//�O�p�`�̃f�[�^�����Z�b�g
		DataReset();

		//�O�p�`�̃f�[�^���Z�b�g
		m_triangleData = triangleData;

		//�C���f�b�N�X�̒l�̍��v���v�Z
		m_sumOfIndexes = m_triangleData.vertexIndexes[0] + m_triangleData.vertexIndexes[1] + m_triangleData.vertexIndexes[2];

		//�O�p�`�̒��_�����ɕ\�A���A�ʏ�ɃO���[�v��������B
		VertexGrouping();

		//�O�p�`����������Ă��邩����B
		//���ʂɂ���ĕʁX��TriangleMaker���p�������I�u�W�F�N�g���Ă΂��
		ITriangleMaker* triangleMaker = CheckHowDivided();

		//���_�̃f�[�^���Z�b�g
		triangleMaker->SetData(&m_newpointArray, m_diagonalPoint, &m_vertexIndexesPack, &m_triangleData);

		//���ʂ��i�[����z����Z�b�g
		triangleMaker->SetArray(m_frontIndexBuffer, m_backIndexBuffer);

		//�C���f�b�N�X�o�b�t�@���쐬�B
		triangleMaker->MakeTriangles();

		//�g�p���I�����TriangleMaker���폜
		delete triangleMaker;
	}

	ITriangleMaker* TriangleDivider::CheckHowDivided()
	{
		//�\���A�����A���ʏ�ɂ������_�����邩���擾
		int frontSize = static_cast<int>(m_vertexIndexesPack.frontVertexIndexes.size());
		int backSize = static_cast<int>(m_vertexIndexesPack.backVertexIndexes.size());
		int onPlaneSize = static_cast<int>(m_vertexIndexesPack.onPlaneVertexIndexes.size());

		//�񕪊�_�\���ɒ��_3��
		if (frontSize == 3) { return new ThreeOnFrontTriangle; }

		//�񕪊�_�����ɒ��_3��
		if (backSize == 3){ return new ThreeOnBackTriangle; }

		//����_�ʏ�ɒ��_3��
		if (onPlaneSize == 3)
		{ 
			return new ThreeOnPlaneTriangle;
		}

		//����_�\���ɒ��_2��
		if (frontSize == 2 && backSize == 1)
		{
			//��������Ă���̂ŕ����_���v�Z
			GetDividedPoint();
			return new TwoOnFrontTriangle;
		}

		//����_�����ɒ��_2��
		if (frontSize == 1 && backSize == 2)
		{
			//��������Ă���̂ŕ����_���v�Z
			GetDividedPoint();
			return new TwoOnBackTriangle;
		}

		//����_�ʏ�ɒ��_1��
		if (frontSize == 1 && backSize == 1 && onPlaneSize == 1)
		{
			//��������Ă���̂ŕ����_���v�Z
			GetDividedPoint();

			//NOTE:�ؒf�ʐ����̂��߂̉�����
			InsertSortedPairToSet(m_newpointArray[1],m_diagonalPoint);

			return new OneOnPlaneTriangle;
		}

		//NOTE:�ؒf�ʐ����̂��߂̉�����
		if (m_vertexIndexesPack.onPlaneVertexIndexes.size() == 2)
		{
			InsertSortedPairToSet(m_vertexIndexesPack.onPlaneVertexIndexes[0], m_vertexIndexesPack.onPlaneVertexIndexes[1]);
		}

		if (frontSize > backSize)
		{
			//�񕪊�_�\���ɒ��_3��
			return new ThreeOnFrontTriangle;
		}
		else
		{
			//�񕪊�_�����ɒ��_3��
			return new ThreeOnBackTriangle;
		}
	}

	void TriangleDivider::VertexGrouping()
	{
		//���ʏ�̓_����e���_�ւ̌��������߂�
		Vector3 toVertexDir[3];
		toVertexDir[0] = m_vertexBuffer->at(m_triangleData.vertexIndexes[0]).pos - m_planeData.GetPoint();
		toVertexDir[1] = m_vertexBuffer->at(m_triangleData.vertexIndexes[1]).pos - m_planeData.GetPoint();
		toVertexDir[2] = m_vertexBuffer->at(m_triangleData.vertexIndexes[2]).pos - m_planeData.GetPoint();
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
				m_vertexIndexesPack.frontVertexIndexes.push_back(m_triangleData.vertexIndexes[i]);
			}
			else if (angle < 0.0f - FLT_EPSILON)//0��菬���� = ���̒��_�͗��ʑ��ɂ���
			{
				m_vertexIndexesPack.backVertexIndexes.push_back(m_triangleData.vertexIndexes[i]);
			}
			else//0 = ���̒��_�͕��ʏ�ɂ���
			{
				m_vertexIndexesPack.onPlaneVertexIndexes.push_back(m_triangleData.vertexIndexes[i]);
			}
		}
	}

	uint16_t TriangleDivider::GetCrossPoint(const TkmFile::SVertex& startVertex, const TkmFile::SVertex& endVertex)
	{
		//�J�n���_�ƏI�����_�̍��W���擾
		Vector3 startPoint = startVertex.pos;
		Vector3 endPoint = endVertex.pos;
		
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
		
		//���`��ԂŐV�������_�𐶐�
		TkmFile::SVertex newVertex = TkmFile::lerpVertex(lerpRate, startVertex, endVertex);

		//pos�����`�⊮���Ă���̂��������ɂȂ邩������Ȃ��̂Ōv�Z����crossPoint���g�p
		newVertex.pos = crossPoint;

		//���_�o�b�t�@���o�^����Ă��邩����
		auto knownVertexIndex = m_newVertexMap.find(newVertex);
		if (knownVertexIndex != m_newVertexMap.end())
		{
			//�o�^����Ă���΂��̃C���f�b�N�X��Ԃ�
			return knownVertexIndex->second;
		}

		//���_�o�b�t�@�̌��݂̃T�C�Y(���ɒǉ�����v�f�̃C���f�b�N�X�ԍ�)���擾
		uint16_t newVertexIndex = static_cast<uint16_t>(m_vertexBuffer->size());

		//�V���_�̘A�z�z��ɒǉ�
		m_newVertexMap.insert(std::make_pair(newVertex, newVertexIndex));

		//�V�������_�o�b�t�@�ɒǉ�
		m_vertexBuffer->push_back(newVertex);

		return newVertexIndex;
	}

	void TriangleDivider::GetDividedPoint()
	{
		//�������_�̊i�[�C���f�b�N�X
		int pointIndex = 0;

		//�������镽�ʏ�ɎO�p�`�̒��_���������ꍇ�������_�ɒǉ�
		for (auto& onPlaneVertexIndex : m_vertexIndexesPack.onPlaneVertexIndexes)
		{
			//�������_�ɒǉ�
			m_newpointArray[pointIndex] = onPlaneVertexIndex;

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
				uint16_t newVertexIndex = GetCrossPoint(
					m_vertexBuffer->at(frontVertexIndex),
					m_vertexBuffer->at(backVertexIndex));

				m_newpointArray[pointIndex] = newVertexIndex;

				//���ꂪ�ŏ��̕������_�Ȃ�
				if (m_alreadyGetAnyDividePoint == false)
				{
					//���������̂Ɏg���Ă��Ȃ����_�@�܂�l�p�`�𐶐��������V�������_�ƑΊp������钸�_�̃C���f�b�N�X���擾
					m_diagonalPoint = GetLeftoverOfThree(frontVertexIndex, backVertexIndex);

					m_alreadyGetAnyDividePoint = true;
				}

				//�������_�̊i�[�C���f�b�N�X�𑝂₷
				pointIndex++;
			}
		}

		//NOTE:�ؒf�ʐ����̂��߂̉�����
		if (m_newpointArray[0] != m_newpointArray[1])
		{
			InsertSortedPairToSet(m_newpointArray[0], m_newpointArray[1]);
		}
	}
}