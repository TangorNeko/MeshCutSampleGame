#include "stdafx.h"
#include "TriangleDivider.h"
#include "TriangleMakerHeader.h"

namespace Util
{
	bool TriangleDivider::DivideCheck(const TriangleData& triangleData)
	{
		if (m_isDataSet == false)
		{
			MessageBoxA(nullptr, "切断判定前のデータの初期化がされていません。", "エラー", MB_OK);
			std::abort();
		}
		//三角形のデータをリセット
		DataReset();

		//三角形のデータをセット
		m_triangleData = triangleData;

		//三角形の頂点を元に表、裏、面上にグループ分けする。
		VertexGrouping();

		return IsDivide();
	}

	bool TriangleDivider::IsDivide()
	{
		//表側、裏側、平面上にいくつ頂点があるかを取得
		int frontSize = static_cast<int>(m_vertexIndexesPack.frontVertexIndexes.size());
		int backSize = static_cast<int>(m_vertexIndexesPack.backVertexIndexes.size());
		int onPlaneSize = static_cast<int>(m_vertexIndexesPack.onPlaneVertexIndexes.size());

		//片側に3点が寄っているか、面上に3点がある場合分割されていない
		//NOTE:面上に3点がある場合、切断の際は分割されていると判定するが、
		//切断チェックの場合は薄皮1枚だけになる可能性があるので分割していないと判定している。
		if (frontSize == 3 || backSize == 3 || onPlaneSize == 3) 
		{
			return false;
		}

		//面の前後にに1点ずつあった場合分割されている
		if (frontSize >= 1 && backSize >= 1)
		{
			return true;
		}

		//残りはどちらかに2つと面上に1つのパターンなので分割されていない
		return false;
	}

	void TriangleDivider::Divide(const TriangleData& triangleData)
	{
		//データの初期化前に切断が実行された場合エラー。
		if (m_isInited == false)
		{
			MessageBoxA(nullptr, "切断前のデータの初期化がされていません。", "エラー", MB_OK);
			std::abort();
		}
		
		//三角形のデータをリセット
		DataReset();

		//三角形のデータをセット
		m_triangleData = triangleData;

		//インデックスの値の合計を計算
		m_sumOfIndexes = m_triangleData.vertexIndexes[0] + m_triangleData.vertexIndexes[1] + m_triangleData.vertexIndexes[2];

		//三角形の頂点を元に表、裏、面上にグループ分けする。
		VertexGrouping();

		//三角形が分割されているか判定。
		//結果によって別々のTriangleMakerを継承したオブジェクトが呼ばれる
		ITriangleMaker* triangleMaker = CheckHowDivided();

		//頂点のデータをセット
		triangleMaker->SetData(&m_newpointArray, m_diagonalPoint, &m_vertexIndexesPack, &m_triangleData);

		//結果を格納する配列をセット
		triangleMaker->SetArray(m_frontIndexBuffer, m_backIndexBuffer);

		//インデックスバッファを作成。
		triangleMaker->MakeTriangles();

		//使用し終わったTriangleMakerを削除
		delete triangleMaker;
	}

	ITriangleMaker* TriangleDivider::CheckHowDivided()
	{
		//表側、裏側、平面上にいくつ頂点があるかを取得
		int frontSize = static_cast<int>(m_vertexIndexesPack.frontVertexIndexes.size());
		int backSize = static_cast<int>(m_vertexIndexesPack.backVertexIndexes.size());
		int onPlaneSize = static_cast<int>(m_vertexIndexesPack.onPlaneVertexIndexes.size());

		//非分割_表側に頂点3つ
		if (frontSize == 3) { return new ThreeOnFrontTriangle; }

		//非分割_裏側に頂点3つ
		if (backSize == 3){ return new ThreeOnBackTriangle; }

		//特殊_面上に頂点3つ
		if (onPlaneSize == 3)
		{ 
			return new ThreeOnPlaneTriangle;
		}

		//分割_表側に頂点2つ
		if (frontSize == 2 && backSize == 1)
		{
			//分割されているので分割点を計算
			GetDividedPoint();
			return new TwoOnFrontTriangle;
		}

		//分割_裏側に頂点2つ
		if (frontSize == 1 && backSize == 2)
		{
			//分割されているので分割点を計算
			GetDividedPoint();
			return new TwoOnBackTriangle;
		}

		//分割_面上に頂点1つ
		if (frontSize == 1 && backSize == 1 && onPlaneSize == 1)
		{
			//分割されているので分割点を計算
			GetDividedPoint();

			//NOTE:切断面生成のための仮処理
			InsertSortedPairToSet(m_newpointArray[1],m_diagonalPoint);

			return new OneOnPlaneTriangle;
		}

		//NOTE:切断面生成のための仮処理
		if (m_vertexIndexesPack.onPlaneVertexIndexes.size() == 2)
		{
			InsertSortedPairToSet(m_vertexIndexesPack.onPlaneVertexIndexes[0], m_vertexIndexesPack.onPlaneVertexIndexes[1]);
		}

		if (frontSize > backSize)
		{
			//非分割_表側に頂点3つ
			return new ThreeOnFrontTriangle;
		}
		else
		{
			//非分割_裏側に頂点3つ
			return new ThreeOnBackTriangle;
		}
	}

	void TriangleDivider::VertexGrouping()
	{
		//平面上の点から各頂点への向きを求める
		Vector3 toVertexDir[3];
		toVertexDir[0] = m_vertexBuffer->at(m_triangleData.vertexIndexes[0]).pos - m_planeData.GetPoint();
		toVertexDir[1] = m_vertexBuffer->at(m_triangleData.vertexIndexes[1]).pos - m_planeData.GetPoint();
		toVertexDir[2] = m_vertexBuffer->at(m_triangleData.vertexIndexes[2]).pos - m_planeData.GetPoint();
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
				m_vertexIndexesPack.frontVertexIndexes.push_back(m_triangleData.vertexIndexes[i]);
			}
			else if (angle < 0.0f - FLT_EPSILON)//0より小さい = その頂点は裏面側にある
			{
				m_vertexIndexesPack.backVertexIndexes.push_back(m_triangleData.vertexIndexes[i]);
			}
			else//0 = その頂点は平面上にある
			{
				m_vertexIndexesPack.onPlaneVertexIndexes.push_back(m_triangleData.vertexIndexes[i]);
			}
		}
	}

	uint16_t TriangleDivider::GetCrossPoint(const TkmFile::SVertex& startVertex, const TkmFile::SVertex& endVertex)
	{
		//開始頂点と終了頂点の座標を取得
		Vector3 startPoint = startVertex.pos;
		Vector3 endPoint = endVertex.pos;
		
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
		
		//線形補間で新しい頂点を生成
		TkmFile::SVertex newVertex = TkmFile::lerpVertex(lerpRate, startVertex, endVertex);

		//posも線形補完しているのだが同じになるか分からないので計算したcrossPointを使用
		newVertex.pos = crossPoint;

		//頂点バッファが登録されているか検索
		auto knownVertexIndex = m_newVertexMap.find(newVertex);
		if (knownVertexIndex != m_newVertexMap.end())
		{
			//登録されていればそのインデックスを返す
			return knownVertexIndex->second;
		}

		//頂点バッファの現在のサイズ(次に追加する要素のインデックス番号)を取得
		uint16_t newVertexIndex = static_cast<uint16_t>(m_vertexBuffer->size());

		//新頂点の連想配列に追加
		m_newVertexMap.insert(std::make_pair(newVertex, newVertexIndex));

		//新しく頂点バッファに追加
		m_vertexBuffer->push_back(newVertex);

		return newVertexIndex;
	}

	void TriangleDivider::GetDividedPoint()
	{
		//分割頂点の格納インデックス
		int pointIndex = 0;

		//分割する平面上に三角形の頂点があった場合分割頂点に追加
		for (auto& onPlaneVertexIndex : m_vertexIndexesPack.onPlaneVertexIndexes)
		{
			//分割頂点に追加
			m_newpointArray[pointIndex] = onPlaneVertexIndex;

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
				uint16_t newVertexIndex = GetCrossPoint(
					m_vertexBuffer->at(frontVertexIndex),
					m_vertexBuffer->at(backVertexIndex));

				m_newpointArray[pointIndex] = newVertexIndex;

				//これが最初の分割頂点なら
				if (m_alreadyGetAnyDividePoint == false)
				{
					//線分を作るのに使っていない頂点　つまり四角形を生成した時新しい頂点と対角線を作る頂点のインデックスを取得
					m_diagonalPoint = GetLeftoverOfThree(frontVertexIndex, backVertexIndex);

					m_alreadyGetAnyDividePoint = true;
				}

				//分割頂点の格納インデックスを増やす
				pointIndex++;
			}
		}

		//NOTE:切断面生成のための仮処理
		if (m_newpointArray[0] != m_newpointArray[1])
		{
			InsertSortedPairToSet(m_newpointArray[0], m_newpointArray[1]);
		}
	}
}