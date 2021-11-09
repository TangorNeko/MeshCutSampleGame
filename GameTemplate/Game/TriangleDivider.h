#pragma once

#include "tkFile/TkmFile.h"
#include "TriangleDivideStruct.h"
namespace Util
{
	using VertexBuffer = std::vector<TkmFile::SVertex>;
	using IndexBuffer = TkmFile::SIndexbuffer16;
	class ITriangleMaker;

	class TriangleDivider
	{
	public:

		/**
		 * @brief ������
		 * @param[in] planeData �����Ɏg�p���镽�ʂ̃f�[�^
		 * @param[in,out] vertexBufferContainer ��������郁�b�V���̒��_�o�b�t�@
		 * @param[out] frontIndexBuffer ������A�����ʂ̕\���Ɣ��肳�ꂽ���b�V���̃C���f�b�N�X�o�b�t�@
		 * @param[out] backIndexBuffer ������A�����ʂ̗����Ɣ��肳�ꂽ���b�V���̃C���f�b�N�X�o�b�t�@
		 * @param[out] newVertexContainer �V�����������ꂽ���_�̃C���f�b�N�X���i�[����A�z�z��
		*/
		void Init(const PlaneData& planeData,
			std::vector<TkmFile::SVertex>* vertexBufferContainer)
		{
			m_planeData = planeData;
			m_vertexBuffer = vertexBufferContainer;
			m_isDataSet = true;

			m_isInited = m_isDataSet && m_isReceiverSet;
		}

		void InitReceiver(TkmFile::SIndexbuffer16* frontIndexBuffer,
			TkmFile::SIndexbuffer16* backIndexBuffer)
		{
			m_frontIndexBuffer = frontIndexBuffer;
			m_backIndexBuffer = backIndexBuffer;
			m_isReceiverSet = true;

			m_isInited = m_isDataSet && m_isReceiverSet;
		}

		bool DivideCheck(const TriangleData& triangleData);

		/**
		 * @brief �����̎��s(��)�@�K���������ʂ��Z�b�g���Ă���ĂԎ�
		 * @param triangleData[in] ���������s����O�p�`�̃f�[�^
		*/
		void Divide(const TriangleData& triangleData);

		void SetCutSeg(std::set<std::pair<uint16_t, uint16_t>>* cutSurfaceSegmentSet)
		{
			m_cutSurfaceSegmentSet = cutSurfaceSegmentSet;
		}

	private:

		/**
		 * @brief �f�[�^�̃��Z�b�g
		*/
		void DataReset()
		{
			//�������_���擾�������̃t���O�����Z�b�g
			m_alreadyGetAnyDividePoint = false;

			//�ʂ̗��E�\�E��ɑ��݂��钸�_�̃C���f�b�N�X�f�[�^�����Z�b�g
			m_vertexIndexesPack.frontVertexIndexes.clear();
			m_vertexIndexesPack.backVertexIndexes.clear();
			m_vertexIndexesPack.onPlaneVertexIndexes.clear();

			//�V���_�Ίp�̃C���f�b�N�X��������
			m_diagonalPoint = -1;
		}

		bool IsDivide();

		/**
		 * @brief �O�p�`���ǂ���������Ă��邩�𔻒肵�A�K�؂�TriangleMaker��Ԃ�
		 * @return �����̂�����ɉ�����TriangleMaker
		*/
		ITriangleMaker* CheckHowDivided();

		/**
		 * @brief �O�p�`�̒��_���O���[�v��������
		*/
		void VertexGrouping();

		/**
		 * @brief ���ʂƐ����Ƃ̌����n�_�����߂�(�q�b�g���Ă���O��)
		 * @param[in] startPoint �����̊J�n�ʒu
		 * @param[in] endPoint �����̏I���ʒu
		 * @return �����n�_�̒��_�̃C���f�b�N�X
		*/
		uint16_t GetCrossPoint(const TkmFile::SVertex& startVertex, const TkmFile::SVertex& endVertex);

		/**
		 * @brief 0,1,2�̃C���f�b�N�X�ԍ��̂����܂܂�Ă��Ȃ�����Ԃ�
		 * @param[in] first 1�ڂ̃C���f�b�N�X�̐���
		 * @param[in] second 2�ڂ̃C���f�b�N�X�̐���
		 * @return �Ō�Ɏc�����C���f�b�N�X�̐���
		*/
		int GetLeftoverOfThree(int first, int second)
		{
			return m_sumOfIndexes - first - second;
		}

		/**
		 * @brief ���ʂƃO���[�v�������ꂽ���_���番�����ꂽ�����̒��_�����߂�
		 * @param[in] verticesPack ���_�̃f�[�^(�O���[�v������)
		 * @param[out] points ���������̒��_�̔z��(�v�f��2)
		 * @return �l�p�`���ł��鎞�A��ڂ̐V���_�̑Ίp�����\������_�̃C���f�b�N�X
		*/
		void GetDividedPoint();

		void InsertSortedPairToSet(uint16_t i0, uint16_t i1)
		{
			if (i0 < i1)
			{
				m_cutSurfaceSegmentSet->insert(std::make_pair(i0, i1));
			}
			else
			{
				m_cutSurfaceSegmentSet->insert(std::make_pair(i1, i0));
			}
		}
	private:
		bool m_isDataSet = false;
		bool m_isReceiverSet = false;
		bool m_isInited = false;					//����������Ă���?
		bool m_alreadyGetAnyDividePoint = false;	//���łɕ������_��1�ł��擾����?
		int m_sumOfIndexes;							//3�̃C���f�b�N�X�̍��v
		PlaneData m_planeData;						//�������ʃf�[�^
		TriangleData m_triangleData;				//���������O�p�`�̃f�[�^
		VertexIndexesPack m_vertexIndexesPack;		//�ʂ̗��E�\�E��ɑ��݂��钸�_�̃C���f�b�N�X�f�[�^
		std::array<uint16_t, 2> m_newpointArray;	//���ʂƌ��������n�_�̒��_�̃C���f�b�N�X�z��
		int m_diagonalPoint = -1;					//�V���_�̑Ίp�ɂ��钸�_�̃C���f�b�N�X
		VertexBuffer* m_vertexBuffer = nullptr;		//���̒��_�o�b�t�@
		IndexBuffer* m_frontIndexBuffer = nullptr;	//�\���̃��b�V���̃C���f�b�N�X�o�b�t�@
		IndexBuffer* m_backIndexBuffer = nullptr;	//�����̃��b�V���̃C���f�b�N�X�o�b�t�@
		std::map<TkmFile::SVertex, uint16_t> m_newVertexMap;//���_�o�b�t�@���L�[�Ƃ��āA���ɓ������_���o�^����Ă����ꍇ���̃C���f�b�N�X��Ԃ��A�z�z��
		std::set<std::pair<uint16_t, uint16_t>>* m_cutSurfaceSegmentSet = nullptr;//�ؒf�ʂɂł��������̊J�n�A�I���C���f�b�N�X���i�[����W��
	};
}