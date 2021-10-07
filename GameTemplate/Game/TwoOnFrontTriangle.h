#pragma once
#include "ITriangleMaker.h"

namespace Util
{
	class TwoOnFrontTriangle : public ITriangleMaker
	{
		/**
		 * @brief インデックスバッファを構成する
		*/
		void MakeTriangles() override;
	};
}

