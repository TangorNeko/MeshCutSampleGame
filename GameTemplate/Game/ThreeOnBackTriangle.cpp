#include "stdafx.h"
#include "ThreeOnBackTriangle.h"

namespace Util
{
	void ThreeOnBackTriangle::MakeTriangles()
	{
		//表側の三角形はない
		
		//3点が平面より裏側にあるので3点の三角形が裏側の三角形

		//3点で三角形を作成
		BackPushTriangle(m_triangleData->vertexIndexes[0], m_triangleData->vertexIndexes[1], m_triangleData->vertexIndexes[2]);
	}
}
