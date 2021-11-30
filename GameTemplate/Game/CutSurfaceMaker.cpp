#include "stdafx.h"
#include "CutSurfaceMaker.h"
#include "TwoDTriangulate.h"
#include <algorithm>

namespace Util
{
	void CutSurfaceMaker::ConvertFromSet()
	{
		int setVal1, setVal2;

		//線分のインデックスの集合を走査
		for (auto it = m_segmentSet->begin(); it != m_segmentSet->end();it++)
		{
			//線分の始点のインデックスから座標を取得
			Vector3 key = m_vertexContainer->at((*it).first).pos;

			//座標をキーとして連想配列からインデックスを検索
			auto foundIndex = m_vectorMap.find(key);
			if (foundIndex != m_vectorMap.end())
			{
				//見つかったら、インデックスを格納
				setVal1 = (*foundIndex).second;
			}
			else
			{
				//見つからなかったら、座標を新規追加してそのインデックスを格納
				setVal1 = m_vectorContainer.size();
				m_vectorContainer.push_back(key);
				m_vectorMap.insert(std::make_pair(key, setVal1));
			}

			//TODO:firstとsecondが違うだけで繰り返し、分離する?

			//線分の終点のインデックスから座標を取得
			key = m_vertexContainer->at((*it).second).pos;

			//座標をキーとして連想配列からインデックスを検索
			foundIndex = m_vectorMap.find(key);
			if (foundIndex != m_vectorMap.end())
			{
				//見つかったら、インデックスを格納
				setVal2 = (*foundIndex).second;
			}
			else
			{
				//見つからなかったら、座標を新規追加してそのインデックスを格納
				setVal2 = m_vectorContainer.size();
				m_vectorContainer.push_back(key);
				m_vectorMap.insert(std::make_pair(key, setVal2));
			}

			//ソートして変換後の集合に格納
			if (setVal1 < setVal2)
			{
				m_convertedSet.insert(std::make_pair(setVal1, setVal2));
			}
			else if(setVal1 > setVal2)
			{
				m_convertedSet.insert(std::make_pair(setVal2, setVal1));
			}
		}
	}

	void CutSurfaceMaker::MakeLinkFromSet()
	{
		//リンク番号
		int linkIndex = 0;

		//リンクを格納する可変長配列の1枠目を開ける
		m_pointLinkArray.resize(1);
		while (true)
		{
			//変換後の集合を走査
			for (auto it = m_convertedSet.begin();it != m_convertedSet.end();)
			{
				//リンクを格納する可変長配列が空なら(新しいリンクなら)
				if (m_pointLinkArray[linkIndex].size() == 0)
				{
					//1番目と2番目の値を格納
					m_pointLinkArray[linkIndex].push_back((*it).first);
					m_pointLinkArray[linkIndex].push_back((*it).second);

					//格納した値を集合から削除
					it = m_convertedSet.erase(it);
				}
				else//リンクを格納する可変長配列が空ではない(すでにリンクが存在するなら)
				{
					//リンクの最後尾の値を取得
					int linkTail = *(m_pointLinkArray[linkIndex].end() - 1);

					//1番目の値が最後尾の値と一緒なら
					if ((*it).first == linkTail)
					{
						//2番目の値をリンクに追加
						m_pointLinkArray[linkIndex].push_back((*it).second);

						//格納した値を集合から削除
						m_convertedSet.erase(it);

						//イテレーターを最初に戻す
						it = m_convertedSet.begin();
					}
					else if ((*it).second == linkTail)//2番目の値が最後尾の値と一緒なら
					{
						//1番目の値をリンクに追加
						m_pointLinkArray[linkIndex].push_back((*it).first);

						//格納した値を集合から削除
						m_convertedSet.erase(it);

						//イテレーターを最初に戻す
						it = m_convertedSet.begin();
					}
					else
					{
						//イテレーターを増やす
						it++;
					}
				}
			}

			//集合のサイズが0なら(すべてのリンクを作り終えたら)
			if (m_convertedSet.size() == 0)
			{
				break;
			}

			//新たなリンクを作成するのでインデックスを増やす
			linkIndex++;

			//リンク用の可変長配列をリサイズ
			m_pointLinkArray.resize(linkIndex + 1);
		}
	}

	bool CutSurfaceMaker::CalcIn2D(const Vector3& normal)
	{
		//切断面の頂点数が3未満なら切断面を生成する必要がない。
		if (m_vectorContainer.size() < 3)
		{
			return false;
		}
		//可変長配列の0番目の座標から1番目の座標までのベクトルをx軸とした座標系を作成する
		m_ex = m_vectorContainer[1] - m_vectorContainer[0];
		m_ex.Normalize();

		//z軸は切断平面の法線とする
		m_ez = normal;
		m_ez.Normalize();

		//y軸はx軸とz軸の外積で求める
		m_ey = m_ex;
		m_ey.Cross(m_ez);
		m_ey.Normalize();
		
		//基底軸変換の行列を作成
		Matrix transitionMatrix = g_matIdentity;
		transitionMatrix.v[0] = { m_ex.x,m_ex.y,m_ex.z,0 };
		transitionMatrix.v[1] = { m_ey.x,m_ey.y,m_ey.z,0 };
		transitionMatrix.v[2] = { m_ez.x,m_ez.y,m_ez.z,0 };
		transitionMatrix.Inverse();

		//3Dでの平面上の座標を切断面の向きの方向から見た2D空間に変換する

		//連想配列に登録する用の何番目かのインデックス番号
		int index = 0;
		for (auto& pos : m_vectorContainer)
		{
			//m_vectorContainerの0番目を2D空間上の原点とする
			Vector3 calcVec = pos - m_vectorContainer[0];

			//基底軸変換
			transitionMatrix.Apply3x3(calcVec);

			//変換によって3DのZ座標は全て同じになるのでZ軸を除くと2D空間上の座標になる
 			Vector2 converted2DVec = { calcVec.x,calcVec.y };

			//可変長配列に格納
			m_2DArray.push_back(converted2DVec);

			//多角形分割に使用する連想配列にもインデックスと共に格納
			m_2DMap.insert(std::make_pair(converted2DVec,index));

			//インデックスを増やす
			index++;
		}

		return true;
	}

	bool CutSurfaceMaker::IsClockwise(const std::vector<uint16_t> pointLink)
	{
		Vector2 minPoint = { FLT_MAX,FLT_MAX };
		std::vector<uint16_t>::const_iterator minIt;

		for (auto it = pointLink.begin(); it != pointLink.end(); it++)
		{
			//一番Xが小さい(Xが同じならYも小さい)なら
			if (m_2DArray[*it].x <= minPoint.x && m_2DArray[*it].y <= minPoint.y)
			{
				//座標を取得
				minPoint = m_2DArray[*it];

				//一番小さい地点の前後を調べるため、イテレーターも取得
				minIt = it;
			}
		}

		//最小値点の前後を取得
		uint16_t prev, next;
		if (minIt == pointLink.begin())
		{
			//最小地点が最初の要素だった場合、一個前の要素は最後の要素とする
			prev = *(pointLink.end() - 1);
			next = *(minIt + 1);
		}
		else if (minIt == pointLink.end() - 1)
		{
			prev = *(minIt - 1);
			//最小地点が最後の要素だった場合、一個前の要素は最初の要素とする
			next = *(pointLink.begin());
		}
		else
		{
			prev = *(minIt - 1);
			next = *(minIt + 1);
		}

		Vector2 prevPoint = m_2DArray[prev];
		Vector2 nextPoint = m_2DArray[next];
		
		//prevPointからminPointに引いた線より、nextPointが右にあれば時計回り、左にあれば反時計回り
		Vector2 prevToMin, prevToNext;
		prevToMin.Subtract(minPoint, prevPoint);
		prevToNext.Subtract(nextPoint,prevPoint);

		if (Cross(prevToMin, prevToNext) > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void CutSurfaceMaker::MakeSurface(std::vector<TkmFile::SVertex>& vertexBuffer, std::vector<TkmFile::SVertex>& backVertexBuffer,std::vector<TkmFile::SIndexbuffer16>* frontIndexBufferArray, std::vector<TkmFile::SIndexbuffer16>* backIndexBufferArray)
	{

		//格納した座標から多角形分割
		TwoDTriangulate triangulator;
		//連想配列をセット
		triangulator.SetMap(&m_2DMap);

		//新しくできる三角形のインデックスをリンクごとに格納する可変長配列
		std::vector<std::vector<int>> cutPlaneNewIndexBufferArray;

		//多角形分割を行う頂点群
		std::vector<Vector2> triangulateVertexes;

		//各リンクを走査
		for (auto& pointLink : m_pointLinkArray)
		{
			//リンクが確立されているかの判定
			if (*(pointLink.begin()) == *(pointLink.end() - 1))
			{
				pointLink.erase(pointLink.end() - 1);
			}
			else
			{
				//TODO:returnでなく処理を行わない設定?
				//リンクが確立されていなくても断面を作りたい時はあるよね
				continue;
			}


			//リンクが2D座標上で時計回りか反時計周りかの判定
			//多角形分割をするためにはリンクが反時計周りでないといけないため
			//時計回りなら
			if (IsClockwise(pointLink) == true)
			{
				//反転
				std::reverse(pointLink.begin(), pointLink.end());
			}

			//リンクの順番に従って座標を格納
			for (auto pointIndex : pointLink)
			{
				triangulateVertexes.push_back(m_2DArray[pointIndex]);
			}
			//分割
			triangulator.Triangulate(triangulateVertexes);

			//1つのリンク分のインデックスバッファを格納
			cutPlaneNewIndexBufferArray.push_back(triangulator.GetBuffer());

			//頂点群をリセット
			triangulateVertexes.clear();
		}

		//新しく作成する裏側用の空の頂点バッファに元々のモデルの頂点バッファをコピーしておく
		backVertexBuffer = vertexBuffer;

		//頂点バッファの最後の位置(追加した場合のインデックス)を取得
		int startIndex = vertexBuffer.size();
		//ここから頂点の生成
		int uvCount = 0;
		for (auto pos : m_vectorContainer)
		{
			TkmFile::SVertex addVert;
			addVert.pos = pos;


			//TODO:UVは仮
			addVert.uv = Vector2::Zero;
			uvCount++;

			//スキン関係?よく分からない
			addVert.indices[0] = 0;
			addVert.indices[1] = 0;
			addVert.indices[2] = 0;
			addVert.indices[3] = 0;
			addVert.skinWeights = { 0.0f,0.0f,0.0f,0.0f };

			//NOTE:切断面座標系を使用
			//切断面座標系のZ軸は表側向きなので、裏側の切断面の法線はそのまま使用する。
			addVert.normal = m_ez;
			addVert.tangent = m_ex;
			addVert.binormal = m_ey;

			//裏側のモデル用の頂点バッファに格納
			backVertexBuffer.push_back(addVert);

			//表側の切断面の法線が表側向きだと逆になるので反転する。
			addVert.normal *= -1;
			addVert.tangent *= -1;
			addVert.binormal *= -1;

			//表側のモデル用の頂点バッファに格納
			vertexBuffer.push_back(addVert);
		}

		//リンクごとに分けられたインデックスバッファの配列を走査する
		for (const auto& cutPlaneNewIndexBuffer : cutPlaneNewIndexBufferArray)
		{
			for (auto newIndex : cutPlaneNewIndexBuffer)
			{
				//NOTE:仮で0番目のマテリアルのインデックスとして追加
				//もし新しいマテリアルを適用するとすればMaterialを増やす必要がある?
				frontIndexBufferArray->at(0).indices.push_back(newIndex + startIndex);
				backIndexBufferArray->at(0).indices.push_back(newIndex + startIndex);
			}
		}
	}
}