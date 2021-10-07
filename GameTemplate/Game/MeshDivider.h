#pragma once
#include "tkFile/TkmFile.h"

namespace Util
{
	class MeshDivider
	{
		/**
		 * @brief �������郁�b�V���̏�����
		 * @param mesh �������郁�b�V��
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
		std::map<std::pair<Vector3, Vector3>, uint32_t> m_newVertexContainer;	//�V�����ł������_�̃C���f�b�N�X���i�[����A�z�z��
		TkmFile::SIndexBuffer32 m_frontIndexBuffer;
		TkmFile::SIndexBuffer32 m_backIndexBuffer;
	};
}

