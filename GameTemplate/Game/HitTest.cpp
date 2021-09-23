#include "stdafx.h"
#include "HitTest.h"

namespace Util
{
	bool IsPlaneDivideTriangle(const PlaneData& planeData, const TriangleData& triangleData, VerticesPack& verticesPack)
	{
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
			float angle = planeData.GetNormal().Dot(toVertexDir[i]);

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

	Vector3 GetCrossPoint(const PlaneData& planeData, const Vector3& startPoint, const Vector3& endPoint)
	{
		//平面上の一点から各点へのベクトルを求める
		Vector3 toStart = startPoint - planeData.planePoint;

		Vector3 toEnd = endPoint - planeData.planePoint;

		//各点と平面の法線から射影の長さを求める
		float projectionA = planeData.GetNormal().Dot(toStart);

		//もう一つの点は平面の裏側にあるので反転した法線との射影の長さを求める
		Vector3 reverseNormal = planeData.GetNormal() * -1;
		float projectionB = reverseNormal.Dot(toEnd);

		//交差地点は開始地点から終了地点へのベクトルに 
		//スタートへの射影の長さ÷2つの射影の長さ ＝ (全体の長さのうち平面上の一点からスタートへの長さの割合)
		//をかけた物を開始地点に足すと求まる
		Vector3 crossPoint = endPoint - startPoint;
		crossPoint *= projectionA / (projectionA + projectionB);
		crossPoint += startPoint;

		return crossPoint;
	}

	int GetDividedPoint(const PlaneData& planeData, const VerticesPack& verticesPack,std::array<Vector3,2>& points)
	{
		//新しくできた頂点数
		int newPointNum = 0;

		//分割頂点の格納インデックス
		int pointIndex = 0;

		//分割する平面上に三角形の頂点があった場合分割頂点に追加
		for (auto& onPlaneVertices : verticesPack.onPlaneVertices)
		{
			//分割頂点に追加
			points[pointIndex] = onPlaneVertices;
			//分割頂点の格納インデックスは増やすが、既にある頂点なので新しくできた頂点数は増やさない
			pointIndex++;
		}

		//分割面の表側の頂点から裏側の頂点に伸びる線分と分割面との交差点を求める事で新しい頂点の座標が手に入る
		//NOTE:二重for文だが、頂点は合計3つしかなく、片側の頂点数が増えると反対側の頂点数が減ることと
		//分割する平面上に頂点があった場合さらに判定回数が減るため
		//for文の中身が呼ばれる回数は1~2回で固定である
		for (auto& frontVertice : verticesPack.frontVertices)
		{
			for (auto& backVertice : verticesPack.backVertices)
			{
				//表側の頂点から裏側の頂点への線分と平面との交差点を求め、分割頂点に追加
				points[pointIndex] = GetCrossPoint(planeData, frontVertice, backVertice);

				//新しい頂点なので新しくできた頂点数を増やす
				newPointNum++;

				//分割頂点の格納インデックスを増やす
				pointIndex++;
			}
		}

		//新しくできた頂点数を返す
		return newPointNum;
	}
}