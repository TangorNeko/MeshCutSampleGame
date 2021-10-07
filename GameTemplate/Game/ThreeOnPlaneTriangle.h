#pragma once
#include "ITriangleMaker.h"

namespace Util
{
	class ThreeOnPlaneTriangle : public ITriangleMaker
	{
		/**
		 * @brief インデックスバッファを構成する
		*/
		void MakeTriangles() override;
	};
}

