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

		/**
		 * @brief 表側の頂点のうち、指定したインデックス番号でないインデックスを返す
		 * @param diagonalIndex 指定インデックス番号(対角のインデックス番号)
		 * @return 指定したインデックス番号でないインデックス番号
		*/
		uint32_t FrontLeftOf(uint32_t diagonalIndex);
	};
}

