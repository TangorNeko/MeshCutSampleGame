#pragma once

#include "tkFile/TkmFile.h"

namespace Util
{
	/**
	 * @brief ���ʂ̃f�[�^
	*/
	struct PlaneData
	{
		/**
		 * @brief ���ʏ�̈�_���擾
		 * @return ���ʏ�̈�_�̍��W
		*/
		const Vector3& GetPoint() const { return planePoint; }

		/**
		 * @brief ���ʏ�̈�_��ݒ�
		 * @param point ���ʏ�̈�_�̍��W
		*/
		void SetPoint(const Vector3& point)
		{
			planePoint = point;
		}

		/**
		 * @brief �@�����擾
		 * @return �@���̃x�N�g��
		*/
		const Vector3& GetNormal()const  { return planeNormal; }

		/**
		 * @brief �@����ݒ肵���K��
		 * @param normal �@���̃x�N�g��
		*/
		void SetNormal(const Vector3& normal)
		{
			if (normal.LengthSq() == 0)
			{
				MessageBoxA(nullptr, "�s���Ȗ@�����Z�b�g���Ă��܂��B\n", "�G���[", MB_OK);
				std::abort();
			}
			planeNormal = normal;
			planeNormal.Normalize();
		}
	private:
		Vector3 planePoint = Vector3::Zero;		//���ʂ̈�_
		Vector3 planeNormal = Vector3::Right;	//���ʂ̖@��
	};

	/**
	 * @brief �O�p�`�̃f�[�^
	*/
	struct TriangleData
	{
		uint32_t vertexIndexes[3];					//���_�̃C���f�b�N�X
	};

	/**
	 * @brief ���_�̃f�[�^
	*/
	struct VertexIndexesPack
	{
		std::vector<uint32_t> frontVertexIndexes;	//���ʂ̕\���ɂ��钸�_�̃C���f�b�N�X�̔z��
		std::vector<uint32_t> backVertexIndexes;		//���ʂ̗����ɂ��钸�_�̃C���f�b�N�X�z��
		std::vector<uint32_t> onPlaneVertexIndexes;	//���ʏ�ɂ��钸�_�̃C���f�b�N�X�̔z��
	};

	enum DivideState//�����̂����
	{
		Divided_2OnFront,//����_�\���ɒ��_2��
		Divided_2OnBack,//����_�����ɒ��_2��
		Divided_1OnPlane,//����_�ʏ�ɒ��_1��
		NotDivided_3OnFront,//�񕪊�_�\���ɒ��_3��
		NotDivided_3OnBack,//�񕪊�_�����ɒ��_3��
		Special_3OnPlane,//����_�ʏ�ɒ��_3��
	};

	class TriangleDivider
	{
	public:

		/**
		 * @brief ������
		 * @param planeData �������ʂ̃f�[�^
		 * @param newVertexContainer MeshDivider�̐V���_��
		*/
		void Init(const PlaneData& planeData,
			std::vector<TkmFile::SVertex>* vertexBufferContainer,
			TkmFile::SIndexBuffer32* frontIndexBuffer,
			TkmFile::SIndexBuffer32* backIndexBuffer,
			std::map<std::pair<Vector3, Vector3>, uint32_t>* newVertexContainer)
		{
			m_planeData = planeData;
			m_vertexBufferContainer = vertexBufferContainer;
			m_frontIndexBuffer = frontIndexBuffer;
			m_backIndexBuffer = backIndexBuffer;
			m_newVertexContainer = newVertexContainer;
			m_isInited = true;
		}

		/**
		 * @brief �����̎��s(��)�@�K���������ʂ��Z�b�g���Ă���ĂԎ�
		 * @param triangleData[in] ���������s����O�p�`�̃f�[�^
		*/
		void Divide(const TriangleData& TriangleData);

		//private:

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
		}

		/**
		 * @brief �����̂�������v�Z
		 * @return �����̂����
		*/
		DivideState CalcDivideState();

		/**
		 * @brief ���ʂ��O�p�`�𕪊����Ă���?
		 * @return �����̂����
		*/
		DivideState IsPlaneDivideTriangle();

		/**
		 * @brief ���ʂƐ����Ƃ̌����n�_�����߂�(�q�b�g���Ă���O��)
		 * @param startPoint[in] �����̊J�n�ʒu
		 * @param endPoint[in] �����̏I���ʒu
		 * @return �����n�_�̒��_�̃C���f�b�N�X
		*/
		uint32_t GetCrossPoint(const TkmFile::SVertex& startVertex, const TkmFile::SVertex& endVertex);

		/**
		 * @brief 0,1,2�̃C���f�b�N�X�ԍ��̂����܂܂�Ă��Ȃ�����Ԃ�
		 * @param first 1�ڂ̃C���f�b�N�X�̐���
		 * @param second 2�ڂ̃C���f�b�N�X�̐���
		 * @return �Ō�Ɏc�����C���f�b�N�X�̐���
		*/
		int GetLeftoverOfThree(int first, int second)
		{
			return 3 - first - second;
		}

		/**
		 * @brief ���ʂƃO���[�v�������ꂽ���_���番�����ꂽ�����̒��_�����߂�
		 * @param verticesPack[in] ���_�̃f�[�^(�O���[�v������)
		 * @param points[out] ���������̒��_�̔z��(�v�f��2)
		 * @return �l�p�`���ł��鎞�A��ڂ̐V���_�̑Ίp�����\������_�̃C���f�b�N�X
		*/
		int GetDividedPoint(std::array<uint32_t, 2>& points);

	private:
		bool m_isInited = false;					//����������Ă���?
		bool m_alreadyGetAnyDividePoint = false;	//���łɕ������_��1�ł��擾����?
		PlaneData m_planeData;						//�������ʃf�[�^
		TriangleData m_triangleData;				//���������O�p�`�̃f�[�^
		VertexIndexesPack m_vertexIndexesPack;		//�ʂ̗��E�\�E��ɑ��݂��钸�_�̃C���f�b�N�X�f�[�^
		std::vector<TkmFile::SVertex>* m_vertexBufferContainer;	//���̒��_�o�b�t�@
		std::map<std::pair<Vector3, Vector3>, uint32_t>* m_newVertexContainer; //�����ɂ���Ăł����V���_���i�[����A�z�z��̃|�C���^
		TkmFile::SIndexBuffer32* m_frontIndexBuffer;
		TkmFile::SIndexBuffer32* m_backIndexBuffer;
	};
}