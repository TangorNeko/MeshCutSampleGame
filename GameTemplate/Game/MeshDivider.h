#pragma once
#include "tkFile/TkmFile.h"

namespace Util
{
	using IndexBuffer = TkmFile::SIndexbuffer16;

	class MeshDivider
	{
	public:
		/**
		 * @brief �������郁�b�V���̏�����
		 * @param[in] mesh �������郁�b�V��
		*/
		void Init(TkmFile::SMesh* mesh)
		{
			m_divideMesh = mesh;
		}

		/**
		 * @brief ���b�V���𕪒f���Ă��邩�̃`�F�b�N
		 * @param cutNormal �ؒf�ʏ�̈�_����̖@���̌���
		 * @param cutPoint �ؒf�ʏ�̈�_�̍��W
		 * @return 
		*/
		bool DivideCheck(const Vector3& cutNormal, const Vector3& cutPoint);

		/**
		 * @brief ���b�V���̕���
		 * @param cutNormal �ؒf�ʏ�̈�_����̖@���̌���
		 * @param cutPoint �ؒf�ʏ�̈�_�̍��W
		 * @return 
		*/
		std::pair<TkmFile::SMesh, TkmFile::SMesh> Divide(const Vector3& cutNormal, const Vector3& cutPoint);

	private:
		TkmFile::SMesh* m_divideMesh = nullptr;//�������郁�b�V��
		IndexBuffer m_frontIndexBuffer;		//�\���̃��b�V���̃C���f�b�N�X�o�b�t�@
		IndexBuffer m_backIndexBuffer;		//�����̃��b�V���̃C���f�b�N�X�o�b�t�@
	};
}

