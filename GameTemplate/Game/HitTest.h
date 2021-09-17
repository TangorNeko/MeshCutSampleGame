#pragma once

namespace Util
{
	struct PlaneData
	{
		Vector3 planePoint = Vector3::Zero;		//平面の一点
		Vector3 planeNormal = Vector3::Right;	//平面の法線
	};

	struct TriangleData
	{
		Vector3 vertices[3];					//頂点
	};

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
}