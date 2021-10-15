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
}