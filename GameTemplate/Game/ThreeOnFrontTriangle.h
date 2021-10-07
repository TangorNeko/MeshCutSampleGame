#pragma once
#include "ITriangleMaker.h"

namespace Util
{
	class ThreeOnFrontTriangle : public ITriangleMaker
	{
		/**
		 * @brief インデックスバッファを構成する
		*/
		void MakeTriangles() override;
	};
}

