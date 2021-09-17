#include "stdafx.h"
#include "HitTest.h"

namespace Util
{
	bool IsPlaneDivideTriangle(const PlaneData& planeData, const TriangleData& triangleData, VerticesPack& verticesPack)
	{
		//平面の法線を取得
		Vector3 planeNormal = planeData.planeNormal;
		planeNormal.Normalize();


		//平面上の点から各頂点への向きを求める
		Vector3 toVertexDir[3];
		toVertexDir[0] = triangleData.vertices[0] - planeData.planePoint;
		toVertexDir[1] = triangleData.vertices[1] - planeData.planePoint;
		toVertexDir[2] = triangleData.vertices[2] - planeData.planePoint;
		for (Vector3& toVertex : toVertexDir)
		{
			toVertex.Normalize();
		}


		//三角形の各点が平面の表裏どっちに存在するかを調べる

		//各配列の領域を確保(頂点は3つなので最大でも3つまでしか入らない)
		//表面側にあったときに格納する配列
		verticesPack.frontVertices.reserve(3);
		//裏面側にあったときに格納する配列
		verticesPack.backVertices.reserve(3);
		//面上にあったときに格納する配列
		verticesPack.onPlaneVertices.reserve(3);

		//各点への向きと平面の法線を調べ、グループ分けする。
		for (int i = 0; i < 3; i++)
		{
			float angle = planeNormal.Dot(toVertexDir[i]);

			if (angle > 0.0f + FLT_EPSILON)//0より大きい = その頂点は表面側にある
			{
				verticesPack.frontVertices.push_back(triangleData.vertices[i]);
			}
			else if (angle < 0.0f - FLT_EPSILON)//0より小さい = その頂点は裏面側にある
			{
				verticesPack.backVertices.push_back(triangleData.vertices[i]);
			}
			else//0 = その頂点は平面上にある
			{
				verticesPack.onPlaneVertices.push_back(triangleData.vertices[i]);
			}
		}

		//表面と裏面に一つ以上ずつ頂点があれば平面は三角形を分割している
		if (verticesPack.frontVertices.size() >= 1 && verticesPack.backVertices.size() >= 1)
		{
			return true;
		}

		return false;
	}
}