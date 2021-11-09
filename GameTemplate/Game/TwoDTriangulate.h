#pragma once

/**
 * @brief 多角形分割のクラス
* 以下のページのプログラムをを利用しています。
* http://www.prefield.com/algorithm/geometry/triangulate.html
*/
class TwoDTriangulate
{
	/**
	 * @brief 点a,b,cの順番に進む時
	 * @param a 点aの座標
	 * @param b 点bの座標
	 * @param c 点cの座標
	 * @return 
		+1:a-bの後反時計回り方向にb-c
		-1:a-bの後時計回り方向にb-c
		+2:a-bの後逆を向いてaをまたぐ形でb-c　	(c-a-b)
		-2:a-bの後そのまま直進してb-c			(a-b-c)
		 0:a-bの後逆を向いてb-c					(a-c-b)
	*/
	int CCW(Vector2 a, Vector2 b, Vector2 c);

	/**
	 * @brief 三点から三角形を作成
	 * @param a 点aの座標
	 * @param b 点bの座標
	 * @param c 点cの座標
	 * @return 点1,2,3を一つのコンテナにまとめた物
	*/
	std::vector<Vector2> MakeTriangle(const Vector2& a, const Vector2& b, const Vector2& c);

	/**
	 * @brief インデックスバッファに三角形を構成する点を追加する
	 * @param a 点aの座標
	 * @param b 点bの座標
	 * @param c 点cの座標
	*/
	void PushIndex(const Vector2& a, const Vector2& b, const Vector2 c);

	/**
	 * @brief 点Pが三角形の中に含まれるか?
	 * @param tri 三角形のデータ
	 * @param p 点Pの座標
	 * @return 
	*/
	bool TriangleContains(const std::vector<Vector2>& tri, const Vector2& p);

	/**
	 * @brief 耳分解?
	*/
	bool EarQ(int i, int j, int k, const std::vector<Vector2>& P);

public:
	/**
	 * @brief 点の座標とインデックスとの関係を示した連想配列を格納する
	 * @param TDMap 連想配列
	*/
	void SetMap(std::map<Vector2, int>* TDMap)
	{
		m_2DMap = TDMap;
	}

	/**
	 * @brief 多角形分割
	 * @param P 分割する点群
	*/
	void Triangulate(const std::vector<Vector2>& P);

	/**
	 * @brief 切断面のインデックスバッファを取得
	 * @return インデックスバッファ
	*/
	std::vector<int> GetBuffer()
	{
		return m_indexBuffer;
	}
private:
	std::map<Vector2, int>* m_2DMap = nullptr;	//モデルの頂点の2D座標とそのインデックスを関連付けた連想配列
	std::vector<int> m_indexBuffer;				//切断面のインデックスバッファ
};

