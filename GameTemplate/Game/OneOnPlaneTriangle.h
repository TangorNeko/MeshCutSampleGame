#pragma once
#include "ITriangleMaker.h"

namespace Util
{
	class OneOnPlaneTriangle : public ITriangleMaker
	{
		/**
		 * @brief インデックスバッファを構成する
		*/
		void MakeTriangles() override;
	};
}

