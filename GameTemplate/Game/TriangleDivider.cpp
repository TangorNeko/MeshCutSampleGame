#include "stdafx.h"
#include "TriangleDivider.h"

namespace Util
{
	void TriangleDivider::Divide(const TriangleData& triangleData)
	{
		if (m_isInited == false)
		{
			MessageBoxA(nullptr, "切断前のデータの初期化がされていません。", "エラー", MB_OK);
			std::abort();
		}
		
		DataReset();
		m_triangleData = triangleData;

		std::array<uint32_t, 2> newpointArray;
		uint32_t diagonal;

		//分割判定の結果によって分岐?
		//TODO:新オブジェクトとして分岐させたほうがコード量的によさそう
		switch (IsPlaneDivideTriangle())
		{
		case Divided_2OnFront:
			diagonal = GetDividedPoint(newpointArray);
			//1つ目の新頂点と対角の頂点でできた線分と、残り2つの頂点をそれぞれつないだ三角形2つが表側の三角形
			m_frontIndexBuffer->indices.push_back(newpointArray[0]);//新頂点1
			m_frontIndexBuffer->indices.push_back(diagonal);//新頂点1の対角の頂点
			m_frontIndexBuffer->indices.push_back(1);//残りの頂点の1つめ(TODO:後から変更)

			m_frontIndexBuffer->indices.push_back(newpointArray[0]);//1つめの新頂点
			m_frontIndexBuffer->indices.push_back(diagonal);//対角の頂点
			m_frontIndexBuffer->indices.push_back(2);//残りの頂点の2つめ(TODO:後から変更)

			//裏の頂点と新頂点2つで裏側の三角形
			m_backIndexBuffer->indices.push_back(newpointArray[0]);//新頂点1
			m_backIndexBuffer->indices.push_back(newpointArray[1]);//新頂点2
			m_backIndexBuffer->indices.push_back(m_vertexIndexesPack.backVertexIndexes[0]);//裏の頂点(1つしかない)
			break;
		case Divided_2OnBack:
			diagonal = GetDividedPoint(newpointArray);
			//表の頂点と新頂点2つで表側の三角形
			m_frontIndexBuffer->indices.push_back(newpointArray[0]);//新頂点1
			m_frontIndexBuffer->indices.push_back(newpointArray[1]);//新頂点2
			m_frontIndexBuffer->indices.push_back(m_vertexIndexesPack.frontVertexIndexes[0]);//表の頂点(1つしかない)

			//1つ目の新頂点と対角の頂点でできた線分と、残り2つの頂点をそれぞれつないだ三角形2つが裏側の三角形
			m_backIndexBuffer->indices.push_back(newpointArray[0]);//新頂点1
			m_backIndexBuffer->indices.push_back(diagonal);//新頂点1の対角の頂点
			m_backIndexBuffer->indices.push_back(1);//残りの頂点の1つめ(TODO:後から変更)

			m_backIndexBuffer->indices.push_back(newpointArray[0]);//新頂点1
			m_backIndexBuffer->indices.push_back(diagonal);//新頂点1の対角の頂点
			m_backIndexBuffer->indices.push_back(2);//残りの頂点の2つめ(TODO:後から変更)
			break;
		case Divided_1OnPlane:
			diagonal = GetDividedPoint(newpointArray);
			//1つ目の新頂点には平面と重なった元の点が入っている。
			//2つ目の新頂点と対角の頂点でできた線分と、表側の1頂点をつないだ三角形が表側の三角形
			m_frontIndexBuffer->indices.push_back(newpointArray[1]);//新頂点2
			m_frontIndexBuffer->indices.push_back(diagonal);//新頂点2の対角の頂点
			m_frontIndexBuffer->indices.push_back(m_vertexIndexesPack.frontVertexIndexes[0]);//表の頂点(1つしかない)

			//2つ目の新頂点と対角の頂点でできた線分と、裏側の1頂点をつないだ三角形が裏側の三角形
			m_backIndexBuffer->indices.push_back(newpointArray[1]);//新頂点2
			m_backIndexBuffer->indices.push_back(diagonal);//新頂点2の対角の頂点
			m_backIndexBuffer->indices.push_back(m_vertexIndexesPack.backVertexIndexes[0]);//裏の頂点(1つしかない)
			break;
		case NotDivided_3OnFront:
			//3点が平面より表側にあるので3点の三角形が表側の三角形
			m_frontIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[0]);
			m_frontIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[1]);
			m_frontIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[2]);
			//裏側の三角形はない
			break;
		case NotDivided_3OnBack:
			//表側の三角形はない
			//3点が平面より裏側にあるので3点の三角形が裏側の三角形
			m_backIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[0]);
			m_backIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[1]);
			m_backIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[2]);
			break;
		case Special_3OnPlane:
			//NOTE:平面上に3点がある時はどちらの面にも含んでいる処理とすることにしている。
			//3点の三角形で表側の三角形
			m_frontIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[0]);
			m_frontIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[1]);
			m_frontIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[2]);

			//3点の三角形で裏側の三角形
			m_backIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[0]);
			m_backIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[1]);
			m_backIndexBuffer->indices.push_back(m_triangleData.vertexIndexes[2]);
			break;
		}
	}

	DivideState TriangleDivider::CalcDivideState()
	{
		int frontSize = m_vertexIndexesPack.frontVertexIndexes.size();
		int backSize = m_vertexIndexesPack.backVertexIndexes.size();
		int onPlaneSize = m_vertexIndexesPack.onPlaneVertexIndexes.size();

		if (frontSize == 3)
		{
			return NotDivided_3OnFront;
		}

		if (backSize == 3)
		{
			return NotDivided_3OnBack;
		}

		if (onPlaneSize == 3)
		{
			return Special_3OnPlane;
		}

		if (frontSize == 2 && backSize == 1)
		{
			return Divided_2OnFront;
		}

		if (frontSize == 1 && backSize == 2)
		{
			return Divided_2OnBack;
		}

		if (frontSize == 1 && backSize == 1 && onPlaneSize == 1)
		{
			return Divided_1OnPlane;
		}

		if (frontSize > backSize)
		{
			return NotDivided_3OnFront;
		}
		else
		{
			return NotDivided_3OnBack;
		}
	}

	DivideState TriangleDivider::IsPlaneDivideTriangle()
	{
		//平面上の点から各頂点への向きを求める
		Vector3 toVertexDir[3];
		toVertexDir[0] = m_vertexBufferContainer->at(m_triangleData.vertexIndexes[0]).pos - m_planeData.GetPoint();
		toVertexDir[1] = m_vertexBufferContainer->at(m_triangleData.vertexIndexes[1]).pos - m_planeData.GetPoint();
		toVertexDir[2] = m_vertexBufferContainer->at(m_triangleData.vertexIndexes[2]).pos - m_planeData.GetPoint();
		for (Vector3& toVertex : toVertexDir)
		{
			toVertex.Normalize();
		}


		//三角形の各点が平面の表裏どっちに存在するかを調べる

		//各配列の領域を確保(頂点は3つなので最大でも3つまでしか入らない)
		//表面側にあったときに格納する配列
		m_vertexIndexesPack.frontVertexIndexes.reserve(3);
		//裏面側にあったときに格納する配列
		m_vertexIndexesPack.backVertexIndexes.reserve(3);
		//面上にあったときに格納する配列
		m_vertexIndexesPack.onPlaneVertexIndexes.reserve(3);

		//各点への向きと平面の法線を調べ、グループ分けする。
		for (int i = 0; i < 3; i++)
		{
			float angle = m_planeData.GetNormal().Dot(toVertexDir[i]);

			if (angle > 0.0f + FLT_EPSILON)//0より大きい = その頂点は表面側にある
			{
				m_vertexIndexesPack.frontVertexIndexes.push_back(i);
			}
			else if (angle < 0.0f - FLT_EPSILON)//0より小さい = その頂点は裏面側にある
			{
				m_vertexIndexesPack.backVertexIndexes.push_back(i);
			}
			else//0 = その頂点は平面上にある
			{
				m_vertexIndexesPack.onPlaneVertexIndexes.push_back(i);
			}
		}

		//頂点の分かれ方を取得
		return CalcDivideState();
	}

	uint32_t TriangleDivider::GetCrossPoint(const TkmFile::SVertex& startVertex, const TkmFile::SVertex& endVertex)
	{
		//開始頂点と終了頂点の座標を取得
		Vector3 startPoint = startVertex.pos;
		Vector3 endPoint = endVertex.pos;

		//startPointとendPointが逆の場合結果は同じなのにmap上では別になるので、
		//ソートされたペアを作成する
		std::pair<Vector3,Vector3> sortedPair = Vector3::MakeSortedPair(startPoint,endPoint);

		//すでに連想配列に結果が格納されている場合return
		auto knownVertexIndex = m_newVertexContainer->find(sortedPair);
		if (knownVertexIndex != m_newVertexContainer->end())
		{
			return knownVertexIndex->second;
		}
		

		//平面上の一点から各点へのベクトルを求める
		Vector3 toStart = startPoint - m_planeData.GetPoint();

		Vector3 toEnd = endPoint - m_planeData.GetPoint();

		//各点と平面の法線から射影の長さを求める
		float projectionA = m_planeData.GetNormal().Dot(toStart);

		//もう一つの点は平面の裏側にあるので反転した法線との射影の長さを求める
		Vector3 reverseNormal = m_planeData.GetNormal() * -1;
		float projectionB = reverseNormal.Dot(toEnd);

		//交差地点は開始地点から終了地点へのベクトルに 
		//スタートへの射影の長さ÷2つの射影の長さ ＝ (全体の長さのうち平面上の一点からスタートへの長さの割合)
		//をかけた物を開始地点に足すと求まる
		Vector3 crossPoint = endPoint - startPoint;
		crossPoint *= projectionA / (projectionA + projectionB);
		crossPoint += startPoint;

		//線形補間率を計算
		Vector3 startToEnd = endPoint - startPoint;
		Vector3 startToCrossPoint = crossPoint - startPoint;
		float lerpRate = startToCrossPoint.Length() / startToEnd.Length();
		
		TkmFile::SVertex newVertex = TkmFile::lerpVertex(lerpRate, startVertex, endVertex);
		//posも線形補完しているのだが同じになるか分からないので計算したcrossPointを使用
		newVertex.pos = crossPoint;

		//現在のサイズ(次に追加する要素のインデックス番号)を取得
		uint32_t newVertexIndex = m_vertexBufferContainer->size();
		m_vertexBufferContainer->push_back(newVertex);

		//新しくできた頂点のインデックスを格納する連想配列に挿入
		m_newVertexContainer->insert(std::make_pair(sortedPair, newVertexIndex));

		return newVertexIndex;
	}

	int TriangleDivider::GetDividedPoint(std::array<uint32_t,2>& points)
	{
		//新しくできた頂点数
		int diagonalPointIndex = 0;

		//分割頂点の格納インデックス
		int pointIndex = 0;

		//分割する平面上に三角形の頂点があった場合分割頂点に追加
		for (auto& onPlaneVertexIndex : m_vertexIndexesPack.onPlaneVertexIndexes)
		{
			//分割頂点に追加
			points[pointIndex] = m_triangleData.vertexIndexes[onPlaneVertexIndex];

			//分割頂点の格納インデックスを増やす。
			pointIndex++;
		}

		//分割面の表側の頂点から裏側の頂点に伸びる線分と分割面との交差点を求める事で新しい頂点の座標が手に入る
		//NOTE:二重for文だが、頂点は合計3つしかなく、片側の頂点数が増えると反対側の頂点数が減ることと
		//分割する平面上に頂点があった場合さらに判定回数が減るため
		//for文の中身が呼ばれる回数は1~2回で固定である
		for (auto& frontVertexIndex : m_vertexIndexesPack.frontVertexIndexes)
		{
			for (auto& backVertexIndex : m_vertexIndexesPack.backVertexIndexes)
			{
				//表側の頂点から裏側の頂点への線分と平面との交差点を求め、分割頂点に追加
				uint32_t newVertexIndex = GetCrossPoint(
					m_vertexBufferContainer->at(m_triangleData.vertexIndexes[frontVertexIndex]),
					m_vertexBufferContainer->at(m_triangleData.vertexIndexes[backVertexIndex]));

				points[pointIndex] = newVertexIndex;

				
				//これが最初の分割頂点なら
				if (m_alreadyGetAnyDividePoint == false)
				{
					//線分を作るのに使っていない頂点　つまり四角形を生成した時新しい頂点と対角線を作る頂点のインデックスを取得
					diagonalPointIndex = GetLeftoverOfThree(frontVertexIndex, backVertexIndex);

					m_alreadyGetAnyDividePoint = true;
				}

				//分割頂点の格納インデックスを増やす
				pointIndex++;
			}
		}

		//新しくできた頂点の対角のインデックスを返す
		return diagonalPointIndex;
	}
}