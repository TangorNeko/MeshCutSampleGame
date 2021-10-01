#include "stdafx.h"
#include "MeshDivider.h"
#include "TriangleDivider.h"

namespace Util
{
	void MeshDivider::Divide()
	{
		TriangleDivider triangleDivider;

		PlaneData plane;
		plane.SetNormal(Vector3::Up);
		plane.SetPoint(Vector3::Zero);
		triangleDivider.Init(plane,&m_newVertexContainer);

		//TODO:インデックスバッファと頂点バッファを受け取り
		// インデックスバッファの一つの三角形ごとにTriangleDividerを実行?
		//イメージ
		//for (auto index : IndexBuffer)
		//{
		//	TriangleData triangleData;
		//	triangleData.vertices[0] = VertexBuffer[index[0]];
		//	triangleData.vertices[1] = VertexBuffer[index[1]];
		//	triangleData.vertices[2] = VertexBuffer[index[2]];
		//
		//	triangleDivider.Divide(triangleData);
		//}

	}
}
