#pragma once
#include "tkFile/TkmFile.h"

namespace Util
{
	using IndexBuffer = TkmFile::SIndexBuffer32;
	using NewPointMap = std::map<std::pair<Vector3, Vector3>, uint32_t>;

	class MeshDivider
	{
		/**
		 * @brief �������郁�b�V���̏�����
		 * @param[in] mesh �������郁�b�V��
		*/
		void Init(TkmFile::SMesh* mesh)
		{
			m_divideMesh = mesh;
		}

		/**
		 * @brief ���b�V���̕���(��)
		*/
		void Divide();

	private:
		TkmFile::SMesh* m_divideMesh = nullptr;//�������郁�b�V��
		//WARNING:map�Ɏg�p���邽�߂�����Vector3���m�̔�r���Z�q��K���ɒ�`���Ă���̂ŁA��r�̌��ʂ��������Ȃ��\��������B
		//m_newVertexContainer����ŃG���[���N�����ۂ͂܂������������邱�ƁB
		NewPointMap m_newVertexContainer;	//�V�����ł������_�̃C���f�b�N�X���i�[����A�z�z��
		IndexBuffer m_frontIndexBuffer;		//�\���̃��b�V���̃C���f�b�N�X�o�b�t�@
		IndexBuffer m_backIndexBuffer;		//�����̃��b�V���̃C���f�b�N�X�o�b�t�@
	};
}

