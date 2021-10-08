#pragma once
#include "ITriangleMaker.h"

namespace Util
{
	class TwoOnFrontTriangle : public ITriangleMaker
	{
		/**
		 * @brief �C���f�b�N�X�o�b�t�@���\������
		*/
		void MakeTriangles() override;

		/**
		 * @brief �\���̒��_�̂����A�w�肵���C���f�b�N�X�ԍ��łȂ��C���f�b�N�X��Ԃ�
		 * @param diagonalIndex �w��C���f�b�N�X�ԍ�(�Ίp�̃C���f�b�N�X�ԍ�)
		 * @return �w�肵���C���f�b�N�X�ԍ��łȂ��C���f�b�N�X�ԍ�
		*/
		uint32_t FrontLeftOf(uint32_t diagonalIndex);
	};
}

