#include "stdafx.h"
#include "CutSurfaceMaker.h"

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

	void CutSurfaceMaker::CalcIn2D(const Vector3& normal)
	{
		//可変長配列の0番目の座標から1番目の座標までのベクトルをx軸とした座標系を作成する
		Vector3 ex = m_vectorContainer[1] - m_vectorContainer[0];
		ex.Normalize();

		//z軸は切断平面の法線とする
		Vector3 ez = normal;
		ez.Normalize();

		//y軸はx軸とz軸の外積で求める
		Vector3 ey = ex;
		ey.Cross(ez);
		ey.Normalize();
		
		//基底軸変換の行列を作成
		Matrix transitionMatrix = g_matIdentity;
		transitionMatrix.v[0] = { ex.x,ex.y,ex.z,0 };
		transitionMatrix.v[1] = { ey.x,ey.y,ey.z,0 };
		transitionMatrix.v[2] = { ez.x,ez.y,ez.z,0 };
		transitionMatrix.Inverse();

		//3Dでの平面上の座標を切断面の向きの方向から見た2D空間に変換する
		for (auto& pos : m_vectorContainer)
		{
			//m_vectorContainerの0番目を2D空間上の原点とする
			Vector3 calcVec = pos - m_vectorContainer[0];
			transitionMatrix.Apply3x3(calcVec);

			//この変換によって3DのZ座標は全て同じになるのでZ軸を除くと2D空間上の座標になる
 			Vector2 converted2DVec = { calcVec.x,calcVec.y };

			//可変長配列に格納
			m_2DArray.push_back(converted2DVec);
		}
	}
}