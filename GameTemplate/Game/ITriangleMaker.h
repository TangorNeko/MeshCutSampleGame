#pragma once

#include "tkFile/TkmFile.h"
#include "TriangleDivideStruct.h"

namespace Util
{
	using NewPointArray = std::array<uint32_t, 2>;

	class ITriangleMaker
	{
	public:
		ITriangleMaker() {}

		virtual ~ITriangleMaker() {}

		/**
		 * @brief �O�p�`����邽�߂̃f�[�^���Z�b�g
		 * @param[in] newpointArray �V���_�̃C���f�b�N�X�̔z��
		 * @param[in] diagonal 1�߂̐V���_�̑Ίp�ɂ��钸�_�̃C���f�b�N�X
		 * @param[in] vertexIndexesPack �������ꂽ���_�̃C���f�b�N�X
		 * @param[in] triangleData �O�p�`�̃f�[�^
		*/
		void SetData(NewPointArray* newpointArray, int diagonal, VertexIndexesPack* vertexIndexesPack, TriangleData* triangleData)
		{
			m_newpointArray = newpointArray;
			m_diagonal = diagonal;
			m_vertexIndexesPack = vertexIndexesPack;
			m_triangleData = triangleData;
		}

		/**
		 * @brief ������̃f�[�^���󂯎��p�̃C���f�b�N�X�o�b�t�@���Z�b�g
		 * @param[out] frontIndexBuffer �\���̃��b�V���̃C���f�b�N�X�o�b�t�@
		 * @param[out] backIndexBuffer �����̃��b�V���̃C���f�b�N�X�o�b�t�@
		*/
		void SetArray(TkmFile::SIndexBuffer32* frontIndexBuffer, TkmFile::SIndexBuffer32* backIndexBuffer)
		{
			m_frontIndexBuffer = frontIndexBuffer;
			m_frontIndexBuffer = frontIndexBuffer;
		}

		/**
		 * @brief �C���f�b�N�X�o�b�t�@���\������
		*/
		virtual void MakeTriangles() = 0;
	protected:
		NewPointArray* m_newpointArray = nullptr;				//�V���_�̃C���f�b�N�X�̔z��
		int m_diagonal = -1;									//1�߂̐V���_�̑Ίp�ɂ��钸�_�̃C���f�b�N�X
		VertexIndexesPack* m_vertexIndexesPack = nullptr;		//�������ꂽ���_�̃C���f�b�N�X
		TriangleData* m_triangleData = nullptr;					//�O�p�`�̃f�[�^

		TkmFile::SIndexBuffer32* m_frontIndexBuffer = nullptr;	//�\���̃��b�V���̃C���f�b�N�X�o�b�t�@
		TkmFile::SIndexBuffer32* m_backIndexBuffer = nullptr;	//�����̃��b�V���̃C���f�b�N�X�o�b�t�@
	};
}