#pragma once
#include "ITriangleMaker.h"

namespace Util
{
	class ThreeOnFrontTriangle : public ITriangleMaker
	{
		/**
		 * @brief �C���f�b�N�X�o�b�t�@���\������
		*/
		void MakeTriangles() override;
	};
}

