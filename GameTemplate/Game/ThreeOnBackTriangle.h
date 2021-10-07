#pragma once
#include "ITriangleMaker.h"

namespace Util
{
    class ThreeOnBackTriangle : public ITriangleMaker
    {
        /**
         * @brief インデックスバッファを構成する
        */
        void MakeTriangles() override;
    };
}

