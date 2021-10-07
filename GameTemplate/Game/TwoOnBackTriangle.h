#pragma once
#include "ITriangleMaker.h"

namespace Util
{
	class TwoOnBackTriangle : public ITriangleMaker
	{
		/**
		 * @brief インデックスバッファを構成する
		*/
		void MakeTriangles() override;
	};
}

