#pragma once

namespace Util
{
	/**
	 * @brief 平面のデータ
	*/
	struct PlaneData
	{
		Vector3 planePoint = Vector3::Zero;		//平面の一点
		/**
		 * @brief 法線を設定し正規化
		 * @param normal 法線のベクトル
		*/
		void SetNormal(const Vector3& normal)
		{
			if (normal.LengthSq() == 0)
			{
				MessageBoxA(nullptr, "不正な法線をセットしています。\n", "エラー", MB_OK);
				std::abort();
			}
			planeNormal = normal;
			planeNormal.Normalize();
		}
		/**
		 * @brief 法線を取得
		 * @return 法線のベクトル
		*/
		const Vector3& GetNormal()const  { return planeNormal; }
	private:
		Vector3 planeNormal = Vector3::Right;	//平面の法線
	};

	/**
	 * @brief 三角形のデータ
	*/
	struct TriangleData
	{
		Vector3 vertices[3];					//頂点
	};

	/**
	 * @brief 頂点のデータ
	*/
	struct VerticesPack
	{
		std::vector<Vector3> frontVertices;		//平面の表側にある頂点の配列
		std::vector<Vector3> backVertices;		//平面の裏側にある頂点の配列
		std::vector<Vector3> onPlaneVertices;	//平面上にある頂点の配列
	};


	/**
	 * @brief 平面が三角形を分割している?
	 * @param[in] planeData 平面のデータ
	 * @param[in] triangleData 三角形のデータ
	 * @param[out] verticesPack 頂点のデータ
	 * @return 分割している:true していない:false
	*/
	bool IsPlaneDivideTriangle(const PlaneData& planeData, const TriangleData& triangleData, VerticesPack& verticesPack);

	/**
	 * @brief 平面と線分との交差地点を求める(ヒットしている前提)
	 * @param planeData[in] 平面のデータ
	 * @param startPoint[in] 線分の開始位置
	 * @param endPoint[in] 線分の終了位置
	 * @return 交差地点
	*/
	Vector3 GetCrossPoint(const PlaneData& planeData, const Vector3& startPoint, const Vector3& endPoint);

	/**
	 * @brief 平面とグループ分けされた頂点から分割された部分の頂点を求める
	 * @param planeData[in] 平面のデータ
	 * @param verticesPack[in] 頂点のデータ(グループ分け済)
	 * @param points[out] 分割部分の頂点の配列(要素数2) 
	 * @return 分割された際新しくできた頂点の数
	*/
	int GetDividedPoint(const PlaneData& planeData, const VerticesPack& verticesPack, std::array<Vector3, 2>& points);
}